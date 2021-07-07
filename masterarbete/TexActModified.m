(* ::Package:: *)

(************************************************************************)
(* This file was generated automatically by the Mathematica front end.  *)
(* It contains Initialization cells from a Notebook file, which         *)
(* typically will have the same name as this file except ending in      *)
(* ".nb" instead of ".m".                                               *)
(*                                                                      *)
(* This file is intended to be loaded into the Mathematica kernel using *)
(* the package loading commands Get or Needs.  Doing so is equivalent   *)
(* to using the Evaluate Initialization Cells menu command in the front *)
(* end.                                                                 *)
(*                                                                      *)
(* DO NOT EDIT THIS FILE.  This entire file is regenerated              *)
(* automatically each time the parent Notebook file is saved in the     *)
(* Mathematica front end.  Any changes you make to this file will be    *)
(* overwritten.                                                         *)
(************************************************************************)



(* ::Input::Initialization:: *)
SetDirectory[NotebookDirectory[]];
SetOptions[InputNotebook[],AutoGeneratedPackage->Automatic]
<<xAct`TexAct`
SetOptions[DefFundSpinOperators,ShowValenceInfo->False]; 


(* ::Input::Initialization:: *)
$TexSmallFraction="\\frac";


(* ::Input::Initialization:: *)
Options[TexPrintAlignedEquations]={Labels->False,LastPunctuation->"."};


(* ::Input::Initialization:: *)
TexPrintAlignedEquations[eqlist:{eqs___},OptionsPattern[]]=.


(* ::Input::Initialization:: *)
TexPrintAlignedEquations2[eqlist:{eqs___},uselabelsq_,lastPunctuation_,labellist_ ]:=Module[{TexLHS=TexPrint[xAct`TexAct`Private`LHSpart@#]&/@eqlist,TexRHS=TexPrint[xAct`TexAct`Private`RHSpart@#]&/@eqlist,splittableRHSPositions,splittedRHS,AllWidths,LHSWidths,RHSWidths,MaxRHSWidth,pagewidth,RHSBreakPositions},
TexLHS=(StringJoin[#,"={}"]&/@TexLHS);
TexRHS=TexBreak[#,1,TexBreakBy->"Term",TexBreakString->"\n"]&/@TexRHS;
splittableRHSPositions=xAct`TexAct`Private`SplittingFunction[#,TexBreakAt/.Options[TexBreak],TexBreakInParenthesis/.Options[TexBreak]]&/@TexRHS;
splittedRHS=StringTake[#1,Thread[{Prepend[#2,1],Append[#2-1,StringLength@#1]}]]&@@@(Thread[{TexRHS,splittableRHSPositions}]);
AllWidths=xAct`TexAct`Private`TexWidths@@Join[TexLHS,Join@@splittedRHS];
pagewidth=First@AllWidths;
LHSWidths=Take[Rest@AllWidths,Length@TexLHS];
RHSWidths=xAct`TexAct`Private`partitionRagged[Drop[AllWidths,1+Length@TexLHS],Length/@splittedRHS];
MaxRHSWidth=Round[($TexPrintPageWidth/.latextextwidth->pagewidth)-Max@LHSWidths];
RHSBreakPositions=xAct`TexAct`Private`PointBreakingFunction[{MaxRHSWidth},MaxRHSWidth,#1,pagewidth,#2]&@@@(Thread[{RHSWidths,splittableRHSPositions}]);
TexRHS=StringInsert[#1,"\\nonumber\\\\\n&",#2]&@@@(Thread[{TexRHS,RHSBreakPositions}]);
TexRHS=StringReplace[#,"\n\\nonumber"->"\\nonumber"]&/@TexRHS;
StringJoin["\\begin{align}\n",StringReplace[StringJoin[Riffle[MapThread[StringJoin[#1,"&",#2, If[uselabelsq,StringJoin[" \\label{",#3,"}"],""]]&,{TexLHS,TexRHS,labellist}],",\\\\\n"]],",\\\\\n="-> "\\\\\n="],lastPunctuation<>"\n\\end{align}"]]


(* ::Input::Initialization:: *)
TexPrintAlignedEquations[eqlist:{eqs___},OptionsPattern[]]:=Module[{eqnames={ReleaseHold[ToString/@HoldForm/@Hold[eqs]]},labellist},
labellist=If[And[OptionValue[Labels],NameQ[#]],TexLabelName[#],""]&/@eqnames;
TexPrintAlignedEquations2[eqlist,OptionValue[Labels],OptionValue[LastPunctuation],labellist]]


(* ::Input::Initialization:: *)
TexPrintAlignedEquations[eqlist_,OptionsPattern[]]:=Module[{eqlistname=TexLabelName@ToString@HoldForm[eqlist],labellist},
labellist=StringJoin[eqlistname,ToString[#]]&/@Range[Length[eqlist]];
TexPrintAlignedEquations2[eqlist,OptionValue[Labels],OptionValue[LastPunctuation],labellist]
]/;And[NameQ@ToString@HoldForm[eqlist],Head[eqlist]==List]
