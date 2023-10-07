*************************************************************************
This is a heavily modified version of mpeg_encode, version 1.5b, for
inclusion in Gmsh. The code was made to compile in C++ and modified so
that all errors throw exceptions.

FIXME: When used as a subroutine the code clearly leaks memory (see
e.g. frametype.cpp and param.cpp). This should be fixed...
*************************************************************************

                 MPEG-1 Video Software Encoder
                (Version 1.5; February 1, 1995)

     Lawrence A. Rowe, Kevin Gong, Eugene Hung, Ketan Patel, Steve Smoot
       and Dan Wallach
    Computer Science Division-EECS, Univ. of Calif. at Berkeley

This directory contains the freely distributed Berkeley MPEG-1 Video 
Encoder.  The encoder implements the standard described in the ISO/IEC
International Standard 11172-2.  The code has been compiled and tested 
on the following platforms:

 DECstation 5000 and Alpha
 HP PA-RISC (HP/UX 9.X) (i.e., HP 9000/7XX and 9000/3XX)
 SGI Indigo running IRIX 5.0.1
 Sun Sparc (SunOS 4.X)

In addition, Rainer Menes from the Technical University of Munich has
ported the encoder and decoder to the Macintosh.  You can get that code
directly from him (menes@statistik.tu-muenchen.de), or from the 
Berkeley FTP archive (mm-ftp.CS.Berkeley.EDU).  If you decide to port 
the code to a new architecture, please let us know so that we can 
incorporate the changes into our sources.

This directory contains everything required to build the encoder
and run it.  We have included source code, makefiles, binaries
for selected platforms, documentation, and test data.  Installation 
instructions are given in the file named src/mpeg_encode/INSTALL.  A man 
page is given in the file doc/mpeg_encode.1.  A detailed user 
manual is provided in postscript format in the file doc/user-manual.ps.

The encoder will accept any input file format as long as you provide 
a script to convert the images to PPM, YUV, JPEG, or JMOVIE format.  Input 
file processing is described in the file doc/INPUT.FORMAT.  Options to 
control input file processing and compression parameters are specified in 
a parameter file.  Very little error processing is done when reading 
this file.  We suggest you start with the sample parameter file 
examples/template.param and modify it.  See also examples/default.param.

The convert directory of Mpeg-Tools contains utilities you might find 
useful including: 

programs to do PPM/YUV conversion and programs to convert Parallax
XVideo JPEG files into PPM, YUV, or JPEG frames.

The motion vector search window can be specified, including half-pixel
block matching, in the parameter file.  We have implemented several 
search algorithms for P-frames including: 1) exhaustive search, 
2) subsampled search, and 3) logarithmic search.  We have also implemented
several alternatives for B-frame block matching including: 1) interpolate
best forward and best backward block, 2) find backward block for best
forward or vice-versa (called CROSS2), and 3) exhaustive cross product
(i.e., go out for coffee and a donut!). The search algorithms are controlled
by options in the parameters file.  For tips on choosing the right search
technique, see the user manual.

The encoder can be run on one computer (i.e., sequential) or on several
computers (i.e., parallel).  Our goal is to produce a portable, easy-to-use
encoder that we can use to encode large volumes of video material for
the Berkeley VOD system (see paper VodsProp93.ps.Z on the FTP archive).
The parallelism is done on a sequence of pictures.  In other words, you 
can spawn one or more children to encode continuous runs pictures. The 
uncompressed data can be accessed either through NFS or TCP sockets.  
The goal is to allow you to encode using multiple processors, think 
spare cycles on workstations, to speed up the encoding time.  Although
performance depends on the speed of individual processors, the file system
and network, and the P/B frame search methods, we have encoded 3.75
frames/second on 8 HP Snakes running in parallel as compared with 0.6
frames/second on 1 Snake.  These are preliminary results. We are continuing 
to experiment with and tune the code.  Instructions to run the parallel system 
are given in the man page and the parallel.param example parameter file.

We have done some tuning to produce a reasonable encoder, but there are
many more optimizations that we would like to incorporate.  These 
extensions are listed in the file doc/EXTENSIONS.  If you succeed in 
implementing any of them, please let us know! 

Send bug reports to:

mpeg-bugs@CS.Berkeley.EDU
   Problems, questions, or patches should be sent to this address.

Anyone interested in providing financial support for this research or 
discussing other aspects of this project should contact Larry Rowe at 
Rowe@CS.Berkeley.EDU (+1 510-642-5117).

This software is freely distributed.  That means, you may use it for 
any non-commercial purpose.  However, patents are held by several companies 
on various aspects of the MPEG video standard.  Companies or individuals
who want to develop commercial products that include this code must
acquire licenses from these companies.  For information on licensing, see
Appendix F in the standard.

ACKNOWLEDGEMENTS:

We gratefully thank Hewlett-Packard and Fujitsu who provided financial
support for this work.  We also want to thank the following people and
organizations for their help:

    Jef Poskanzer who developed the pbmplus package.
    ---------
    Copyright (C) 1989, 1991 by Jef Poskanzer.

    Permission to use, copy, modify, and distribute this software and its
    documentation for any purpose and without fee is hereby granted, provided
    that the above copyright notice appear in all copies and that both that
    copyright notice and this permission notice appear in supporting
    documentation.  This software is provided "as is" without express or
    implied warranty.
    ---------

    Eiichi Kowashi of Intel and Avideh Zakhor of U.C. Berkeley who
    provided valuable suggestions on motion vector searching.

    Chad Fogg of the University of Washington who has helped us 
    understand many issues in MPEG coding and decoding.

    Rainer Menes of the Technical University of Munich who has ported the
    the Berkeley MPEG encoder and decoder to the Macintosh, and he has 
    provided us with many suggestions to improve the code.

    Robert Safranek of ATT for comments, suggestions, and most of the
    code for custom quantization tables.

    Jim Boucher of Boston University for jmovie2jpeg.

    The San Diego SuperComputing Center for providing facilities to 
    develop some of the code contained within.
