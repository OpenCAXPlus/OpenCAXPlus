r"""protocols is a module that contains a set of VTK Web related
protocols that can be combined together to provide a flexible way to define
very specific web application.
"""

from __future__ import absolute_import, division, print_function

import os, sys, logging, types, inspect, traceback, re, base64, time

from vtk.vtkWebCore import vtkWebApplication, vtkWebInteractionEvent

# import Twisted reactor for later callback
from twisted.internet import reactor

# from autobahn.wamp import register as exportRpc
from wslink import register as exportRpc
from wslink.websocket import LinkProtocol

# =============================================================================
#
# Base class for any VTK Web based protocol
#
# =============================================================================

class vtkWebProtocol(LinkProtocol):

    def getApplication(self):
        return self.getSharedObject("app")

    # no need for a setApplication anymore, but keep for compatibility
    def setApplication(self, app):
        pass

    def mapIdToObject(self, id):
        """
        Maps global-id for a vtkObject to the vtkObject instance. May return None if the
        id is not valid.
        """
        id = int(id)
        if id <= 0:
            return None
        return self.getApplication().GetObjectIdMap().GetVTKObject(id)

    def getGlobalId(self, obj):
        """
        Return the id for a given vtkObject
        """
        return self.getApplication().GetObjectIdMap().GetGlobalId(obj)

    def getView(self, vid):
        """
        Returns the view for a given view ID, if vid is None then return the
        current active view.
        :param vid: The view ID
        :type vid: str
        """
        view = self.mapIdToObject(vid)

        if not view:
            # Use active view is none provided.
            view = self.getApplication().GetObjectIdMap().GetActiveObject("VIEW")
        if not view:
            raise Exception("no view provided: %s" % vid)

        return view

    def setActiveView(self, view):
        """
        Set a vtkRenderWindow to be the active one
        """
        self.getApplication().GetObjectIdMap().SetActiveObject("VIEW", view)

# =============================================================================
#
# Handle Mouse interaction on any type of view
#
# =============================================================================

class vtkWebMouseHandler(vtkWebProtocol):

    @exportRpc("viewport.mouse.interaction")
    def mouseInteraction(self, event):
        """
        RPC Callback for mouse interactions.
        """
        view = self.getView(event['view'])

        buttons = 0
        if event["buttonLeft"]:
            buttons |= vtkWebInteractionEvent.LEFT_BUTTON;
        if event["buttonMiddle"]:
            buttons |= vtkWebInteractionEvent.MIDDLE_BUTTON;
        if event["buttonRight"]:
            buttons |= vtkWebInteractionEvent.RIGHT_BUTTON;

        modifiers = 0
        if event["shiftKey"]:
            modifiers |= vtkWebInteractionEvent.SHIFT_KEY
        if event["ctrlKey"]:
            modifiers |= vtkWebInteractionEvent.CTRL_KEY
        if event["altKey"]:
            modifiers |= vtkWebInteractionEvent.ALT_KEY
        if event["metaKey"]:
            modifiers |= vtkWebInteractionEvent.META_KEY

        pvevent = vtkWebInteractionEvent()
        pvevent.SetButtons(buttons)
        pvevent.SetModifiers(modifiers)
        if "x" in event:
            pvevent.SetX(event["x"])
        if "y" in event:
            pvevent.SetY(event["y"])
        if "scroll" in event:
            pvevent.SetScroll(event["scroll"])
        if event["action"] == 'dblclick':
            pvevent.SetRepeatCount(2)
        #pvevent.SetKeyCode(event["charCode"])
        retVal = self.getApplication().HandleInteractionEvent(view, pvevent)
        del pvevent

        if retVal:
            self.getApplication().InvokeEvent('PushRender')

        return retVal

# =============================================================================
#
# Basic 3D Viewport API (Camera + Orientation + CenterOfRotation
#
# =============================================================================

class vtkWebViewPort(vtkWebProtocol):

    @exportRpc("viewport.camera.reset")
    def resetCamera(self, viewId):
        """
        RPC callback to reset camera.
        """
        view = self.getView(viewId)
        camera = view.GetRenderer().GetActiveCamera()
        camera.ResetCamera()
        try:
            # FIXME seb: view.CenterOfRotation = camera.GetFocalPoint()
            print ("FIXME")
        except:
            pass

        self.getApplication().InvalidateCache(view)
        self.getApplication().InvokeEvent('PushRender')

        return str(self.getGlobalId(view))

    @exportRpc("viewport.axes.orientation.visibility.update")
    def updateOrientationAxesVisibility(self, viewId, showAxis):
        """
        RPC callback to show/hide OrientationAxis.
        """
        view = self.getView(viewId)
        # FIXME seb: view.OrientationAxesVisibility = (showAxis if 1 else 0);

        self.getApplication().InvalidateCache(view)
        self.getApplication().InvokeEvent('PushRender')

        return str(self.getGlobalId(view))

    @exportRpc("viewport.axes.center.visibility.update")
    def updateCenterAxesVisibility(self, viewId, showAxis):
        """
        RPC callback to show/hide CenterAxesVisibility.
        """
        view = self.getView(viewId)
        # FIXME seb: view.CenterAxesVisibility = (showAxis if 1 else 0);

        self.getApplication().InvalidateCache(view)
        self.getApplication().InvokeEvent('PushRender')

        return str(self.getGlobalId(view))

    @exportRpc("viewport.camera.update")
    def updateCamera(self, view_id, focal_point, view_up, position):
        view = self.getView(view_id)

        camera = view.GetRenderer().GetActiveCamera()
        camera.SetFocalPoint(focal_point)
        camera.SetCameraViewUp(view_up)
        camera.SetCameraPosition(position)
        self.getApplication().InvalidateCache(view)

        self.getApplication().InvokeEvent('PushRender')

# =============================================================================
#
# Provide Image delivery mechanism
#
# =============================================================================

class vtkWebViewPortImageDelivery(vtkWebProtocol):

    @exportRpc("viewport.image.render")
    def stillRender(self, options):
        """
        RPC Callback to render a view and obtain the rendered image.
        """
        beginTime = int(round(time.time() * 1000))
        view = self.getView(options["view"])
        size = [view.GetSize()[0], view.GetSize()[1]]
        # use existing size, overridden only if options["size"] is set.
        resize = size != options.get("size", size)
        if resize:
            size = options["size"]
            if size[0] > 0 and size[1] > 0:
              view.SetSize(size)
        t = 0
        if options and "mtime" in options:
            t = options["mtime"]
        quality = 100
        if options and "quality" in options:
            quality = options["quality"]
        localTime = 0
        if options and "localTime" in options:
            localTime = options["localTime"]
        reply = {}
        app = self.getApplication()
        if t == 0:
            app.InvalidateCache(view)
        reply["image"] = app.StillRenderToString(view, t, quality)
        # Check that we are getting image size we have set. If not, wait until we
        # do. The render call will set the actual window size.
        tries = 10;
        while resize and list(view.GetSize()) != size \
              and size != [0, 0] and tries > 0:
            app.InvalidateCache(view)
            reply["image"] = app.StillRenderToString(view, t, quality)
            tries -= 1

        reply["stale"] = app.GetHasImagesBeingProcessed(view)
        reply["mtime"] = app.GetLastStillRenderToMTime()
        reply["size"] = [view.GetSize()[0], view.GetSize()[1]]
        reply["format"] = "jpeg;base64"
        reply["global_id"] = str(self.getGlobalId(view))
        reply["localTime"] = localTime

        endTime = int(round(time.time() * 1000))
        reply["workTime"] = (endTime - beginTime)

        return reply

# =============================================================================
#
# Provide publish-based Image delivery mechanism
#
# =============================================================================

class vtkWebPublishImageDelivery(vtkWebProtocol):
    def __init__(self, decode=True):
        super(vtkWebPublishImageDelivery, self).__init__()
        # self.context = SynchronizationContext()
        self.trackingViews = {}
        self.lastStaleTime = 0
        self.staleHandlerCount = 0
        self.deltaStaleTimeBeforeRender = 0.5 # 0.5s
        self.decode = decode

    @exportRpc("viewport.image.push")
    def stillRender(self, options):
        """
        RPC Callback to render a view and obtain the rendered image.
        """
        beginTime = int(round(time.time() * 1000))
        view = self.getView(options["view"])
        size = [view.GetSize()[0], view.GetSize()[1]]
        # use existing size, overridden only if options["size"] is set.
        resize = size != options.get("size", size)
        if resize:
            size = options["size"]
            if size[0] > 0 and size[1] > 0:
              view.SetSize(size)
        t = 0
        if options and "mtime" in options:
            t = options["mtime"]
        quality = 100
        if options and "quality" in options:
            quality = options["quality"]
        localTime = 0
        if options and "localTime" in options:
            localTime = options["localTime"]
        reply = {}
        app = self.getApplication()
        if t == 0:
            app.InvalidateCache(view)
        if self.decode:
            stillRender = app.StillRenderToString
        else:
            stillRender = app.StillRenderToBuffer
        reply_image = stillRender(view, t, quality)

        # Check that we are getting image size we have set. If not, wait until we
        # do. The render call will set the actual window size.
        tries = 10;
        while resize and list(view.GetSize()) != size \
              and size != [0, 0] and tries > 0:
            app.InvalidateCache(view)
            reply_image = stillRender(view, t, quality)
            tries -= 1

        reply["stale"] = app.GetHasImagesBeingProcessed(view)
        reply["mtime"] = app.GetLastStillRenderToMTime()
        reply["size"] = [view.GetSize()[0], view.GetSize()[1]]
        reply["memsize"] = reply_image.GetDataSize() if reply_image else 0
        reply["format"] = "jpeg;base64" if self.decode else "jpeg"
        reply["global_id"] = str(self.getGlobalId(view))
        reply["localTime"] = localTime
        if self.decode:
            reply["image"] = reply_image
        else:
            # Convert the vtkUnsignedCharArray into a bytes object, required by Autobahn websockets
            reply["image"] = memoryview(reply_image).tobytes() if reply_image else None

        endTime = int(round(time.time() * 1000))
        reply["workTime"] = (endTime - beginTime)

        return reply

    @exportRpc("viewport.image.push.observer.add")
    def addRenderObserver(self, viewId):
        sView = self.getView(viewId)
        if not sView:
            return { 'error': 'Unable to get view with id %s' % viewId }

        realViewId = sView.GetGlobalIDAsString()

        # Use default arg to capture value of realViewId
        def pushRender(vId = realViewId):
            if not self.trackingViews[vId]["enabled"]:
                return

            reply = self.stillRender({ "view": realViewId, "mtime": self.trackingViews[vId]["mtime"] })
            stale = reply["stale"]
            if reply["image"]:
                # depending on whether the app has encoding enabled:
                if self.decode:
                    reply["image"] = base64.standard_b64decode(reply["image"]);

                reply["image"] = self.addAttachment(reply["image"]);
                reply["format"] = "jpeg"
                # save mtime for next call.
                self.trackingViews[vId]["mtime"] = reply["mtime"]
                # echo back real ID, instead of -1 for 'active'
                reply["id"] = vId
                self.publish('viewport.image.push.subscription', reply)
            if stale:
                self.lastStaleTime = time.time()
                if self.staleHandlerCount == 0:
                    self.staleHandlerCount += 1
                    reactor.callLater(self.deltaStaleTimeBeforeRender, lambda: renderStaleImage())
            else:
                self.lastStaleTime = 0

        def renderStaleImage():
            self.staleHandlerCount -= 1

            if self.lastStaleTime != 0:
                delta = (time.time() - self.lastStaleTime)
                if delta >= self.deltaStaleTimeBeforeRender:
                    pushRender()
                else:
                    self.staleHandlerCount += 1
                    reactor.callLater(self.deltaStaleTimeBeforeRender - delta + 0.001, lambda: renderStaleImage())

        if not realViewId in self.trackingViews:
            observerCallback = lambda *args, **kwargs: pushRender()
            tag = self.getApplication().AddObserver('PushRender', observerCallback)
            # TODO do we need self.getApplication().AddObserver('ResetActiveView', resetActiveView())
            self.trackingViews[realViewId] = { 'tag': tag, 'observerCount': 1, 'mtime': 0, 'enabled': True, 'quality': 100 }
        else:
            # There is an observer on this view already
            self.trackingViews[realViewId]['observerCount'] += 1

        self.publish('viewport.image.push.subscription', pushRender())
        return { 'success': True, 'viewId': realViewId }

    @exportRpc("viewport.image.push.observer.remove")
    def removeRenderObserver(self, viewId):
        sView = self.getView(viewId)
        if not sView:
            return { 'error': 'Unable to get view with id %s' % viewId }

        realViewId = sView.GetGlobalIDAsString()

        observerInfo = None
        if realViewId in self.trackingViews:
            observerInfo = self.trackingViews[realViewId]

        if not observerInfo:
            return { 'error': 'Unable to find subscription for view %s' % realViewId }

        observerInfo['observerCount'] -= 1

        if observerInfo['observerCount'] <= 0:
            self.getApplication().RemoveObserver(observerInfo['tag'])
            del self.trackingViews[realViewId]

        return { 'result': 'success' }

    @exportRpc("viewport.image.push.quality")
    def setViewQuality(self, viewId, quality):
        sView = self.getView(viewId)
        if not sView:
            return { 'error': 'Unable to get view with id %s' % viewId }

        realViewId = sView.GetGlobalIDAsString()
        observerInfo = None
        if realViewId in self.trackingViews:
            observerInfo = self.trackingViews[realViewId]

        if not observerInfo:
            return { 'error': 'Unable to find subscription for view %s' % realViewId }

        observerInfo['quality'] = quality

        return { 'result': 'success' }

    @exportRpc("viewport.image.push.enabled")
    def enableView(self, viewId, enabled):
        sView = self.getView(viewId)
        if not sView:
            return { 'error': 'Unable to get view with id %s' % viewId }

        realViewId = sView.GetGlobalIDAsString()
        observerInfo = None
        if realViewId in self.trackingViews:
            observerInfo = self.trackingViews[realViewId]

        if not observerInfo:
            return { 'error': 'Unable to find subscription for view %s' % realViewId }

        observerInfo['enabled'] = enabled

        return { 'result': 'success' }

    @exportRpc("viewport.image.push.invalidate.cache")
    def invalidateCache(self, viewId):
        sView = self.getView(viewId)
        if not sView:
            return { 'error': 'Unable to get view with id %s' % viewId }
        if hasattr(sView,'SMProxy'):
            sView = sView.SMProxy

        self.getApplication().InvalidateCache(sView)
        self.getApplication().InvokeEvent('PushRender')
        return { 'result': 'success' }

# =============================================================================
#
# Provide Geometry delivery mechanism (WebGL)
#
# =============================================================================

class vtkWebViewPortGeometryDelivery(vtkWebProtocol):

    @exportRpc("viewport.webgl.metadata")
    def getSceneMetaData(self, view_id):
        view  = self.getView(view_id);
        data = self.getApplication().GetWebGLSceneMetaData(view)
        return data

    @exportRpc("viewport.webgl.data")
    def getWebGLData(self, view_id, object_id, part):
        view  = self.getView(view_id)
        data = self.getApplication().GetWebGLBinaryData(view, str(object_id), part-1)
        return data

# =============================================================================
#
# Provide File/Directory listing
#
# =============================================================================

class vtkWebFileBrowser(vtkWebProtocol):

    def __init__(self, basePath, name, excludeRegex=r"^\.|~$|^\$", groupRegex=r"[0-9]+\."):
        """
        Configure the way the WebFile browser will expose the server content.
         - basePath: specify the base directory that we should start with
         - name: Name of that base directory that will show up on the web
         - excludeRegex: Regular expression of what should be excluded from the list of files/directories
        """
        self.baseDirectory = basePath
        self.rootName = name
        self.pattern = re.compile(excludeRegex)
        self.gPattern = re.compile(groupRegex)

    @exportRpc("file.server.directory.list")
    def listServerDirectory(self, relativeDir='.'):
        """
        RPC Callback to list a server directory relative to the basePath
        provided at start-up.
        """
        path = [ self.rootName ]
        if len(relativeDir) > len(self.rootName):
            relativeDir = relativeDir[len(self.rootName)+1:]
            path += relativeDir.replace('\\','/').split('/')

        currentPath = os.path.join(self.baseDirectory, relativeDir)
        result =  { 'label': relativeDir, 'files': [], 'dirs': [], 'groups': [], 'path': path }
        if relativeDir == '.':
            result['label'] = self.rootName
        for file in os.listdir(currentPath):
            if os.path.isfile(os.path.join(currentPath, file)) and not re.search(self.pattern, file):
                result['files'].append({'label': file, 'size': -1})
            elif os.path.isdir(os.path.join(currentPath, file)) and not re.search(self.pattern, file):
                result['dirs'].append(file)

        # Filter files to create groups
        files = result['files']
        files.sort()
        groups = result['groups']
        groupIdx = {}
        filesToRemove = []
        for file in files:
            fileSplit = re.split(self.gPattern, file['label'])
            if len(fileSplit) == 2:
                filesToRemove.append(file)
                gName = '*.'.join(fileSplit)
                if gName in groupIdx:
                    groupIdx[gName]['files'].append(file['label'])
                else:
                    groupIdx[gName] = { 'files' : [file['label']], 'label': gName }
                    groups.append(groupIdx[gName])
        for file in filesToRemove:
            gName = '*.'.join(re.split(self.gPattern, file['label']))
            if len(groupIdx[gName]['files']) > 1:
                files.remove(file)
            else:
                groups.remove(groupIdx[gName])

        return result
