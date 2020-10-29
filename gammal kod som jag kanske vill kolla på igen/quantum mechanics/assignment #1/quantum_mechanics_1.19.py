### 1.19 ###
import numpy as np
np.set_printoptions(precision=3)
from math import isclose

def commute(A,B):
    return A.dot(B) - B.dot(A)

Sx = (1/2)*np.array([[0, 1], [1, 0]])
Sx2 = Sx.dot(Sx)
Sy = (1/2)*np.array([[0, -1j], [1j, 0]])
Sy2 = Sy.dot(Sy)
Sz = (1/2)*np.array([[1, 0], [0, -1]])

D, V = np.linalg.eig(Sz)
v = V[:, D>0] #spin up in z direction (already normalized)
v_dual = v.conj().T
exp_DeltaSx2 = v_dual.dot(Sx2).dot(v) - (v_dual.dot(Sx).dot(v))**2
print('In the z spin up state, <(Delta Sx)²> =', np.real(exp_DeltaSx2)[0][0]) #<(Delta Sx)²> = <Sx²> - <Sx>²


D, V = np.linalg.eig(Sx)
v = V[:, D>0] #spin up in x direction (already normalized)
v_dual = v.conj().T
exp_DeltaSx2 = v_dual.dot(Sx2).dot(v) - (v_dual.dot(Sx).dot(v))**2
exp_DeltaSy2 = v_dual.dot(Sy2).dot(v) - (v_dual.dot(Sy).dot(v))**2
print('In the x spin up state, <(Delta Sx)²> <(Delta Sy)²> =', np.round(np.real(exp_DeltaSx2*exp_DeltaSy2),5)[0][0])
SxSy = commute(Sx,Sy)
exp_SxSy_commutator = v_dual.dot(SxSy).dot(v)
print('In the x spin up state, 1/4 |<[Sx,Sy]>|² =', 1/4 * np.round(np.abs(exp_SxSy_commutator),5)[0][0])

