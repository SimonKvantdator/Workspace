{{TensorPlus[MultScal[mass, K0Coeff20], MultScal[-mass^2/2, M1Coeff20], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][M1Coeff20]], 
    MultScal[-1/2, SymMult[M1Coeff20, 2, 0][PsiCDe]], CurlCDe[M0Coeff]] == 
   ZeroTensor[{-Spin, -Spin}], TensorPlus[MultScal[-mass^2/2, L1Coeff02], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][L1Coeff02]], 
    MultScal[-mass, N0Coeff02], MultScal[-1/2, SymMult[L1Coeff02, 0, 2][
      PsiCDe\[Dagger]]], CurlDgCDe[L0Coeff]] == 
   ZeroTensor[{Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[-mass/2, K0Coeff00], MultScal[mass/2, N0Coeff00], 
    MultScal[-1/2, DivCDe[L0Coeff]]] == ZeroTensor[{}], 
  TensorPlus[MultScal[-mass/2, K0Coeff00], MultScal[mass/2, N0Coeff00], 
    MultScal[-1/2, DivCDe[M0Coeff]]] == ZeroTensor[{}], 
  TensorPlus[MultScal[-mass^2/2, K1Coeff11], MultScal[mass/2, L0Coeff], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][K1Coeff11]], 
    MultScal[-mass, M0Coeff], MultScal[-2/3, SymMult[K1Coeff11, 1, 1][
      PhiCDe]], CurlDgCDe[K0Coeff20], MultScal[-1/2, TwistCDe[K0Coeff00]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass, L0Coeff], MultScal[-mass/2, M0Coeff], 
    MultScal[-mass^2/2, N1Coeff11], MultScal[2, SymMult[LambdaCDe, 0, 0][
      N1Coeff11]], MultScal[-2/3, SymMult[N1Coeff11, 1, 1][PhiCDe]], 
    CurlCDe[N0Coeff02], MultScal[-1/2, TwistCDe[N0Coeff00]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}]}, 
 {TensorPlus[K0Coeff20, MultScal[-mass/2, M1Coeff20], 
    MultScal[-1/3, CurlCDe[K1Coeff11]]] == ZeroTensor[{-Spin, -Spin}], 
  TensorPlus[MultScal[mass/3, K1Coeff11], L0Coeff, 
    MultScal[(-2*mass)/3, N1Coeff11], MultScal[-1/3, CurlCDe[L1Coeff02]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[(2*mass)/3, K1Coeff11], M0Coeff, 
    MultScal[-mass/3, N1Coeff11], MultScal[-1/3, CurlDgCDe[M1Coeff20]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass/2, L1Coeff02], N0Coeff02, 
    MultScal[-1/3, CurlDgCDe[N1Coeff11]]] == 
   ZeroTensor[{Spin\[Dagger], Spin\[Dagger]}], 
  TwistCDe[K1Coeff11] == ZeroTensor[{-Spin, -Spin, -Spin\[Dagger], 
     -Spin\[Dagger]}], TwistCDe[L1Coeff02] == 
   ZeroTensor[{-Spin, -Spin\[Dagger], -Spin\[Dagger], -Spin\[Dagger]}], 
  TwistCDe[M1Coeff20] == ZeroTensor[{-Spin, -Spin, -Spin, -Spin\[Dagger]}], 
  TwistCDe[N1Coeff11] == ZeroTensor[{-Spin, -Spin, -Spin\[Dagger], 
     -Spin\[Dagger]}]}, 
 {K1Coeff31 == ZeroTensor[{-Spin, -Spin, -Spin, Spin\[Dagger]}], 
  L1Coeff22 == ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}], 
  M1Coeff22 == ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}], 
  N1Coeff13 == ZeroTensor[{-Spin, Spin\[Dagger], Spin\[Dagger], 
     Spin\[Dagger]}]}}
