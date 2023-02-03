import torch
import numpy as np
import scipy.io
import h5py
import sklearn.metrics
import argparse
import torch.nn as nn
from scipy.ndimage import gaussian_filter

#################################################
#
# Utilities
#
#################################################
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

class MatReader(object):
    def __init__(self, file_path, to_torch=True, to_cuda=False, to_float=False):
        super(MatReader, self).__init__()

        self.to_torch = to_torch
        self.to_cuda = to_cuda
        self.to_float = to_float

        self.file_path = file_path

        self.data = None
        self.old_mat = None
        self._load_file()

    def _load_file(self):
        try:
            self.data = scipy.io.loadmat(self.file_path)
            self.old_mat = True
        except:
            self.data = h5py.File(self.file_path)
            self.old_mat = False

    def load_file(self, file_path):
        self.file_path = file_path
        self._load_file()

    def read_field(self, field):
        x = self.data[field]

        if not self.old_mat:
            x = x[()]
            x = np.transpose(x, axes=range(len(x.shape) - 1, -1, -1))

        if self.to_float:
            x = x.astype(np.float32)

        if self.to_torch:
            x = torch.from_numpy(x)

            if self.to_cuda:
                x = x.cuda()

        return x

    def set_cuda(self, to_cuda):
        self.to_cuda = to_cuda

    def set_torch(self, to_torch):
        self.to_torch = to_torch

    def set_float(self, to_float):
        self.to_float = to_float
        
# file_path = './data1.mat'
# reader = MatReader(file_path)
# alldata = reader.read_field('M')
# traindata = alldata[0:1600,:]
# testdata = alldata[1600:,:]
def PCA_svd(X, k, center=False):
    center = False
    h = torch.mean(X,dim=0) if center  else torch.mean(X,dim=0)*0
    X_new = X - h
    u, s, v = torch.svd(X_new.t())
    components  = u[:,:k].t()
    return components

def relative_loss(outputs, labels):
    loss = 100*torch.mean(torch.sum((outputs - labels)**2, axis=1) / torch.sum((labels)**2, axis=1), axis=-1)
    return loss



def test(net, loaded_data_x, loaded_data_y):
    net.eval()
    with torch.no_grad():
        inputs=loaded_data_x
        labels=loaded_data_y
        outputs = net.forward(inputs)
        rmse_test = torch.mean(((labels - outputs).pow(2).sum(-1)).pow(.5)  )
    return rmse_test


def adjust_learning_rate(optimizer, epoch, init_lr, N):
    #lr = 1.0 / (epoch + 1)
    lr = init_lr * 0.1 ** (epoch // N)
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr
    return lr
# cop = PCA_svd(traindata,10)
def get_args():
    parser = argparse.ArgumentParser(description='A general training enviroment for Darcy.')

    # For version control
    parser.add_argument('--cgrid','-c', default=20, type=int,
                        help='coarse grid size.')

    # For methods
    parser.add_argument('--fd', default=32,type = int, help='f_d setting')
    parser.add_argument( '--coarle','-l', default=400, type=int,
                         help='N_v')
    return parser.parse_args()
