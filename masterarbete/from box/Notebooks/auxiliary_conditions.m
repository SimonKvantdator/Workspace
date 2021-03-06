{{TensorPlus[MultScal[-1/2*mass, K0Coeff00], MultScal[mass/2, N0Coeff00], 
    MultScal[-1/6*mass, SymMult[K2Coeff22, 2, 2][PhiCDe]], 
    MultScal[1/3, SymMult[L1Coeff22, 2, 2][PhiCDe]], 
    MultScal[1/24, SymMult[L2Coeff13, 1, 3][CurlCDe[PsiCDe\[Dagger]]]], 
    MultScal[-5/24, SymMult[L2Coeff13, 1, 3][CurlDgCDe[PhiCDe]]], 
    MultScal[-1/2, DivCDe[L0Coeff]]] == ZeroTensor[{}], 
  TensorPlus[MultScal[-1/2*mass, K0Coeff00], MultScal[mass/2, N0Coeff00], 
    MultScal[1/3, SymMult[M1Coeff22, 2, 2][PhiCDe]], 
    MultScal[mass/6, SymMult[N2Coeff22, 2, 2][PhiCDe]], 
    MultScal[-5/24, SymMult[M2Coeff31, 3, 1][CurlCDe[PhiCDe]]], 
    MultScal[1/24, SymMult[M2Coeff31, 3, 1][CurlDgCDe[PsiCDe]]], 
    MultScal[-1/2, DivCDe[M0Coeff]]] == ZeroTensor[{}], 
  TensorPlus[MultScal[-1/2*mass^2, K1Coeff11], MultScal[mass/2, L0Coeff], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][K1Coeff11]], 
    MultScal[-mass, M0Coeff], MultScal[-2/3, SymMult[K1Coeff11, 1, 1][
      PhiCDe]], MultScal[-1/2, SymMult[K1Coeff31, 3, 0][PsiCDe]], 
    MultScal[mass/6, SymMult[L2Coeff13, 0, 3][PsiCDe\[Dagger]]], 
    MultScal[5/12, SymMult[K2Coeff22, 2, 1][CurlCDe[PhiCDe]]], 
    CurlDgCDe[K0Coeff20], MultScal[1/6, SymMult[K2Coeff22, 1, 2][
      CurlDgCDe[PhiCDe]]], MultScal[-5/36, SymMult[DivCDe[PhiCDe], 1, 1][
      K2Coeff22]], MultScal[-1/2, TwistCDe[K0Coeff00]], 
    MultScal[9/4, SymMult[TwistCDe[LambdaCDe], 1, 1][K2Coeff22]], 
    MultScal[-1/2, SymMult[K2Coeff22, 2, 2][TwistCDe[PhiCDe]]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass, L0Coeff], MultScal[-1/2*mass, M0Coeff], 
    MultScal[-1/2*mass^2, N1Coeff11], MultScal[2, SymMult[LambdaCDe, 0, 0][
      N1Coeff11]], MultScal[-2/3, SymMult[N1Coeff11, 1, 1][PhiCDe]], 
    MultScal[-1/6*mass, SymMult[M2Coeff31, 3, 0][PsiCDe]], 
    MultScal[-1/2, SymMult[N1Coeff13, 0, 3][PsiCDe\[Dagger]]], 
    CurlCDe[N0Coeff02], MultScal[1/6, SymMult[N2Coeff22, 2, 1][
      CurlCDe[PhiCDe]]], MultScal[5/12, SymMult[N2Coeff22, 1, 2][
      CurlDgCDe[PhiCDe]]], MultScal[-5/36, SymMult[DivCDe[PhiCDe], 1, 1][
      N2Coeff22]], MultScal[9/4, SymMult[TwistCDe[LambdaCDe], 1, 1][
      N2Coeff22]], MultScal[-1/2, TwistCDe[N0Coeff00]], 
    MultScal[-1/2, SymMult[N2Coeff22, 2, 2][TwistCDe[PhiCDe]]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass, K0Coeff20], MultScal[-1/2*mass^2, M1Coeff20], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][M1Coeff20]], 
    MultScal[-1/2, SymMult[M1Coeff20, 2, 0][PsiCDe]], CurlCDe[M0Coeff], 
    MultScal[-5/36, SymMult[DivCDe[PhiCDe], 1, 1][M2Coeff31]], 
    MultScal[-2/3, SymMult[M1Coeff22, 1, 2][PhiCDe]], 
    MultScal[5/12, SymMult[M2Coeff31, 2, 1][CurlCDe[PhiCDe]]], 
    MultScal[1/20, SymMult[M2Coeff31, 2, 1][CurlDgCDe[PsiCDe]]], 
    MultScal[mass/6, SymMult[N2Coeff22, 1, 2][PhiCDe]], 
    MultScal[9/4, SymMult[TwistCDe[LambdaCDe], 1, 1][M2Coeff31]], 
    MultScal[-1/3, SymMult[M2Coeff31, 3, 1][TwistCDe[PsiCDe]]]] == 
   ZeroTensor[{-Spin, -Spin}], TensorPlus[MultScal[-1/2*mass^2, L1Coeff02], 
    MultScal[2, SymMult[LambdaCDe, 0, 0][L1Coeff02]], 
    MultScal[-mass, N0Coeff02], MultScal[-1/2, SymMult[L1Coeff02, 0, 2][
      PsiCDe\[Dagger]]], CurlDgCDe[L0Coeff], MultScal[-5/36, 
     SymMult[DivCDe[PhiCDe], 1, 1][L2Coeff13]], MultScal[-1/6*mass, 
     SymMult[K2Coeff22, 2, 1][PhiCDe]], MultScal[-2/3, 
     SymMult[L1Coeff22, 2, 1][PhiCDe]], MultScal[1/20, 
     SymMult[L2Coeff13, 1, 2][CurlCDe[PsiCDe\[Dagger]]]], 
    MultScal[5/12, SymMult[L2Coeff13, 1, 2][CurlDgCDe[PhiCDe]]], 
    MultScal[9/4, SymMult[TwistCDe[LambdaCDe], 1, 1][L2Coeff13]], 
    MultScal[-1/3, SymMult[L2Coeff13, 1, 3][TwistCDe[PsiCDe\[Dagger]]]]] == 
   ZeroTensor[{Spin\[Dagger], Spin\[Dagger]}]}, 
 {TensorPlus[MultScal[mass/3, K1Coeff11], L0Coeff, 
    MultScal[(-2*mass)/3, N1Coeff11], MultScal[1/3, 
     SymMult[L2Coeff13, 0, 3][PsiCDe\[Dagger]]], 
    MultScal[-1/3, CurlCDe[L1Coeff02]], MultScal[2/3, DivCDe[L1Coeff22]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[(2*mass)/3, K1Coeff11], M0Coeff, 
    MultScal[-1/3*mass, N1Coeff11], MultScal[1/3, SymMult[M2Coeff31, 3, 0][
      PsiCDe]], MultScal[-1/3, CurlDgCDe[M1Coeff20]], 
    MultScal[2/3, DivCDe[M1Coeff22]]] == ZeroTensor[{-Spin, Spin\[Dagger]}], 
  TensorPlus[K0Coeff20, MultScal[-1/2*mass, M1Coeff20], 
    MultScal[-1/3, CurlCDe[K1Coeff11]], MultScal[1/2, DivCDe[K1Coeff31]], 
    MultScal[1/2, SymMult[K2Coeff22, 1, 2][PhiCDe]]] == 
   ZeroTensor[{-Spin, -Spin}], TensorPlus[MultScal[mass/2, L1Coeff02], 
    N0Coeff02, MultScal[-1/3, CurlDgCDe[N1Coeff11]], 
    MultScal[1/2, DivCDe[N1Coeff13]], MultScal[1/2, 
     SymMult[N2Coeff22, 2, 1][PhiCDe]]] == 
   ZeroTensor[{Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[(-2*mass^2)/3, K2Coeff22], MultScal[mass/3, L1Coeff22], 
    MultScal[6, SymMult[LambdaCDe, 0, 0][K2Coeff22]], 
    MultScal[-mass, M1Coeff22], MultScal[-3/4, SymMult[K2Coeff22, 2, 0][
      PsiCDe]], MultScal[-3/4, SymMult[K2Coeff22, 0, 2][PsiCDe\[Dagger]]], 
    CurlDgCDe[K1Coeff31], MultScal[-3, SymMult[K2Coeff22, 1, 1][PhiCDe]], 
    MultScal[-2/3, TwistCDe[K1Coeff11]]] == 
   ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[(-2*mass^2)/3, L2Coeff13], 
    MultScal[6, SymMult[LambdaCDe, 0, 0][L2Coeff13]], 
    MultScal[-mass, N1Coeff13], CurlDgCDe[L1Coeff22], 
    MultScal[-2, SymMult[L2Coeff13, 1, 1][PhiCDe]], 
    MultScal[-2, SymMult[L2Coeff13, 0, 2][PsiCDe\[Dagger]]], 
    MultScal[-1/2, TwistCDe[L1Coeff02]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger], Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[mass, K1Coeff31], MultScal[(-2*mass^2)/3, M2Coeff31], 
    MultScal[6, SymMult[LambdaCDe, 0, 0][M2Coeff31]], CurlCDe[M1Coeff22], 
    MultScal[-2, SymMult[M2Coeff31, 1, 1][PhiCDe]], 
    MultScal[-2, SymMult[M2Coeff31, 2, 0][PsiCDe]], 
    MultScal[-1/2, TwistCDe[M1Coeff20]]] == 
   ZeroTensor[{-Spin, -Spin, -Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass, L1Coeff22], MultScal[-1/3*mass, M1Coeff22], 
    MultScal[(-2*mass^2)/3, N2Coeff22], MultScal[6, 
     SymMult[LambdaCDe, 0, 0][N2Coeff22]], MultScal[-3/4, 
     SymMult[N2Coeff22, 2, 0][PsiCDe]], MultScal[-3/4, 
     SymMult[N2Coeff22, 0, 2][PsiCDe\[Dagger]]], CurlCDe[N1Coeff13], 
    MultScal[-3, SymMult[N2Coeff22, 1, 1][PhiCDe]], 
    MultScal[-2/3, TwistCDe[N1Coeff11]]] == 
   ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}]}, 
 {TensorPlus[K1Coeff31, MultScal[(-2*mass)/3, M2Coeff31], 
    MultScal[-1/2, CurlCDe[K2Coeff22]]] == 
   ZeroTensor[{-Spin, -Spin, -Spin, Spin\[Dagger]}], 
  TensorPlus[MultScal[mass/4, K2Coeff22], L1Coeff22, 
    MultScal[(-3*mass)/4, N2Coeff22], MultScal[-1/2, CurlCDe[L2Coeff13]]] == 
   ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[(3*mass)/4, K2Coeff22], M1Coeff22, 
    MultScal[-1/4*mass, N2Coeff22], MultScal[-1/2, CurlDgCDe[M2Coeff31]]] == 
   ZeroTensor[{-Spin, -Spin, Spin\[Dagger], Spin\[Dagger]}], 
  TensorPlus[MultScal[(2*mass)/3, L2Coeff13], N1Coeff13, 
    MultScal[-1/2, CurlDgCDe[N2Coeff22]]] == 
   ZeroTensor[{-Spin, Spin\[Dagger], Spin\[Dagger], Spin\[Dagger]}], 
  TwistCDe[K2Coeff22] == ZeroTensor[{-Spin, -Spin, -Spin, -Spin\[Dagger], 
     -Spin\[Dagger], -Spin\[Dagger]}], TwistCDe[L2Coeff13] == 
   ZeroTensor[{-Spin, -Spin, -Spin\[Dagger], -Spin\[Dagger], -Spin\[Dagger], 
     -Spin\[Dagger]}], TwistCDe[M2Coeff31] == 
   ZeroTensor[{-Spin, -Spin, -Spin, -Spin, -Spin\[Dagger], -Spin\[Dagger]}], 
  TwistCDe[N2Coeff22] == ZeroTensor[{-Spin, -Spin, -Spin, -Spin\[Dagger], 
     -Spin\[Dagger], -Spin\[Dagger]}]}, 
 {K2Coeff42 == ZeroTensor[{-Spin, -Spin, -Spin, -Spin, Spin\[Dagger], 
     Spin\[Dagger]}], L2Coeff33 == ZeroTensor[{-Spin, -Spin, -Spin, 
     Spin\[Dagger], Spin\[Dagger], Spin\[Dagger]}], 
  M2Coeff33 == ZeroTensor[{-Spin, -Spin, -Spin, Spin\[Dagger], Spin\[Dagger], 
     Spin\[Dagger]}], N2Coeff24 == ZeroTensor[{-Spin, -Spin, Spin\[Dagger], 
     Spin\[Dagger], Spin\[Dagger], Spin\[Dagger]}]}}
