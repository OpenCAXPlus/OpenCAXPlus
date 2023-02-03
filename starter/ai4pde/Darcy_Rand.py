from __future__ import print_function
import argparse
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms
import scipy
import numpy as np
from sklearn.model_selection  import train_test_split
import random
from torch.autograd import Variable
import timeit
import sys
from utils import *
import scipy.io as sio
use_cuda = torch.cuda.is_available()
print('Use GPU?', use_cuda)

class Net(nn.Module):
    def __init__(self, c_grid_size, out_size,coarse_vel,f_d):
        super(Net, self).__init__()
        self.conv11 = nn.Conv2d(1, f_d, 3)
        self.conv22 = nn.Conv2d(f_d, int(f_d/2), 3)
        # pool_window = int(img_rows / coarse_dof)
        self.pool = nn.AvgPool2d((2, 2))

        # flatten
        self.fc0 = nn.Linear(int((64 - 2 - 2) / 2 * ((32 - 2 - 2)/2)) * int(f_d/2), np.square(c_grid_size))
        # reshape
        self.conv1 = nn.Conv2d(1, int(f_d/2), 3)
        self.conv2 = nn.Conv2d(int(f_d/2), f_d, 3)

        # flatter
        left = c_grid_size - (3 - 1) - (3 - 1)
        # coarse_vel = 200
        # coarse_vel = 100
        self.fc1 = nn.Linear(f_d * left * left, coarse_vel)
        self.fc2 = nn.Linear(coarse_vel, out_size)
        self.coarse_dof = c_grid_size
        self.lrelu = nn.LeakyReLU(0.3)
        self.f_d = f_d
        
        # self.lrelu = F.relu

    def forward(self, x):
        x = self.lrelu(self.conv11(x))
        x = self.lrelu(self.conv22(x))
        x = self.pool(x)
        x = x.view(-1, int((64 - 2 - 2) / 2 * ((32 - 2 - 2)/2)) * int(self.f_d/2))
        x = self.lrelu(self.fc0(x))

        # coarse_dof = 10
        
        x = x.view(-1, 1, self.coarse_dof, self.coarse_dof)
        x = self.lrelu(self.conv1(x))
        x = self.lrelu(self.conv2(x))

        x = x.view(x.shape[0], -1)
        x = self.lrelu(self.fc1(x))
        x = self.lrelu(self.fc2(x))

        return x


file_path = './data/LevRand_DarcyNN.mat'
reader = MatReader(file_path,to_cuda = use_cuda)
K_all = reader.read_field('K')
P_all = reader.read_field('P')

K_all = K_all[0:100,:]
P_all = P_all[0:100,:]

# Number of training data
N = 90

K_train = K_all[0:N,:]
K_train = K_train.reshape((K_train.shape[0],1,64,32))

K_test = K_all[N:,:]    
K_test = K_test.reshape((K_test.shape[0],1,64,32))

P_train = P_all[0:N,:]
P_test = P_all[N:,:] 

out_size = P_train.shape[1]
# cgsize = 15 #5, 15
# coarse_vel = 200
# f_d = 64
arg = get_args()
lr = 1e-3
num_epochs = 10 

net = Net(arg.cgrid, out_size,arg.coarle,arg.fd)


net.double()
if use_cuda:
    net.cuda()
optimizer = optim.Adam(net.parameters(), lr=lr, betas=(0.9, 0.999), eps=1e-08, weight_decay=0, amsgrad=False)
#optimizer = optim.SGD(net.parameters(), lr=lr)
start = timeit.default_timer()
acc_hist = []
train_error_hist = []
current_lr = lr
k = 0

for epoch in range(num_epochs):

   
    current_lr = adjust_learning_rate(optimizer, epoch, lr, 200)
    net.train()
    for i in  range(1):
      optimizer.zero_grad()
    
      output = net(K_train)
      loss = relative_loss(output, P_train)
      loss.backward()
      optimizer.step()
    net.eval()
    with torch.no_grad():    
      ou = net(K_test)
      acc = relative_loss(ou,P_test)
   
    print('\nEpoch {}   Loss: {:0.5f}%  lr is {} acc is {:0.3f}'.format(epoch+1, loss, current_lr, acc))  # Acc train: {:0.2f}
    train_error = loss.data.cpu().numpy()
        # test_error = acc.data.cpu().numpy()
    acc_hist.append(acc.data.cpu().numpy())
    train_error_hist.append(train_error)

#if pars.save_model:
filename = './model/Darcy_CNN_Rand_cg_'+str(arg.cgrid)+'_fd_'+str(arg.fd)+'_cl'+str(arg.coarle) +'.pt'
torch.save(net.state_dict(), filename)

end = timeit.default_timer()
print("Sampling Time used: {:0.1f}".format(end - start))


model_size = sum(param.numel() for param in net.parameters())
print('The number of parameters in the model is',model_size)
#sio.savemat('frac_loss_varyK_theta_cg_'+str(arg.cgrid)+'_fd_'+str(arg.fd)+'_cl'+str(arg.coarle)+'.mat', {'loss':[train_error_hist,acc_hist],'para':model_size})
#sio.savemat('frac_result_varyK_theta_cg_'+str(arg.cgrid)+'_fd_'+str(arg.fd)+'_cl'+str(arg.coarle)+'.mat', {'P_test': [outputs0.cpu().detach().numpy(),outputs1.cpu().detach().numpy()],'testerr':acc_hist,'trainerr':train_error_hist})
