; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA/proj_axi_master/solution2/.autopilot/db/a.o.2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@.str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1 ; [#uses=3 type=[6 x i8]*]
@.str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1 ; [#uses=9 type=[1 x i8]*]
@str = internal constant [19 x i8] c"AFAProcessFunction\00" ; [#uses=1 type=[19 x i8]*]
@str1 = internal constant [38 x i8] c"memcpy.AFAProcessFunctionParamBlock.p\00" ; [#uses=1 type=[38 x i8]*]
@str2 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@str3 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@.str4 = internal constant [17 x i8] c"burstread.region\00" ; [#uses=2 type=[17 x i8]*]
@str5 = internal constant [42 x i8] c"memcpy.p.AFAProcessFunctionParamBlock.gep\00" ; [#uses=1 type=[42 x i8]*]
@str6 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@str7 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@.str8 = internal constant [18 x i8] c"burstwrite.region\00" ; [#uses=2 type=[18 x i8]*]

; [#uses=0]
define void @AFAProcessFunction(i32* %p, i8* %srcNet, i8* %dstNet) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %p) nounwind, !map !184
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %srcNet) nounwind, !map !190
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %dstNet) nounwind, !map !196
  call void (...)* @_ssdm_op_SpecTopModule([19 x i8]* @str) nounwind
  %AFAProcessFunctionParamBlock = alloca [64 x i32], align 16 ; [#uses=2 type=[64 x i32]*]
  call void @llvm.dbg.value(metadata !{i32* %p}, i64 0, metadata !200), !dbg !201 ; [debug line = 13:24] [debug variable = p]
  call void @llvm.dbg.value(metadata !{i8* %srcNet}, i64 0, metadata !202), !dbg !203 ; [debug line = 14:20] [debug variable = srcNet]
  call void @llvm.dbg.value(metadata !{i8* %dstNet}, i64 0, metadata !204), !dbg !205 ; [debug line = 15:20] [debug variable = dstNet]
  call void (...)* @_ssdm_op_SpecInterface(i32* %p, [6 x i8]* @.str, i32 0, i32 0, i32 0, i32 64, [1 x i8]* @.str1, [1 x i8]* @.str1, [1 x i8]* @.str1) nounwind, !dbg !206 ; [debug line = 17:1]
  call void (...)* @_ssdm_op_SpecInterface(i8* %srcNet, [6 x i8]* @.str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @.str1, [1 x i8]* @.str1, [1 x i8]* @.str1) nounwind, !dbg !208 ; [debug line = 18:1]
  call void (...)* @_ssdm_op_SpecInterface(i8* %dstNet, [6 x i8]* @.str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @.str1, [1 x i8]* @.str1, [1 x i8]* @.str1) nounwind, !dbg !209 ; [debug line = 19:1]
  call void @llvm.dbg.declare(metadata !{[64 x i32]* %AFAProcessFunctionParamBlock}, metadata !210), !dbg !214 ; [debug line = 23:12] [debug variable = AFAProcessFunctionParamBlock]
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i7 [ 0, %0 ], [ %indvar.next, %burst.rd.body ] ; [#uses=3 type=i7]
  %exitcond = icmp eq i7 %indvar, -64             ; [#uses=1 type=i1]
  %1 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind ; [#uses=0 type=i32]
  br i1 %exitcond, label %burst.wr.header, label %burst.rd.body

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread.rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @.str4) nounwind ; [#uses=1 type=i32]
  %2 = call i32 (...)* @_ssdm_op_SpecBurst(i32* %p, i32 1, i32 64) nounwind ; [#uses=0 type=i32]
  %3 = call i32 (...)* @_ssdm_SpecLoopFlatten(i32 1, [1 x i8]* @str3) nounwind ; [#uses=0 type=i32]
  %4 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind ; [#uses=0 type=i32]
  %5 = call i32 (...)* @_ssdm_op_SpecLoopName([38 x i8]* @str1) nounwind ; [#uses=0 type=i32]
  %indvar.next = add i7 %indvar, 1                ; [#uses=1 type=i7]
  %tmp = zext i7 %indvar to i64, !dbg !215        ; [#uses=2 type=i64] [debug line = 26:3]
  %p.addr = getelementptr i32* %p, i64 %tmp, !dbg !215 ; [#uses=1 type=i32*] [debug line = 26:3]
  %p.load = load i32* %p.addr, align 4, !dbg !215 ; [#uses=1 type=i32] [debug line = 26:3]
  %AFAProcessFunctionParamBlock.addr = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp, !dbg !215 ; [#uses=1 type=i32*] [debug line = 26:3]
  store i32 %p.load, i32* %AFAProcessFunctionParamBlock.addr, align 4, !dbg !215 ; [debug line = 26:3]
  %burstread.rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @.str4, i32 %burstread.rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.rd.header

burst.wr.header:                                  ; preds = %burst.wr.body, %burst.rd.header
  %indvar4 = phi i7 [ %indvar.next5, %burst.wr.body ], [ 0, %burst.rd.header ] ; [#uses=3 type=i7]
  %exitcond6 = icmp eq i7 %indvar4, -64           ; [#uses=1 type=i1]
  %6 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind ; [#uses=0 type=i32]
  br i1 %exitcond6, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite.rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @.str8) nounwind ; [#uses=1 type=i32]
  %7 = call i32 (...)* @_ssdm_op_SpecBurst(i32* %p, i32 0, i32 64) nounwind ; [#uses=0 type=i32]
  %8 = call i32 (...)* @_ssdm_SpecLoopFlatten(i32 1, [1 x i8]* @str7) nounwind ; [#uses=0 type=i32]
  %9 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind ; [#uses=0 type=i32]
  %10 = call i32 (...)* @_ssdm_op_SpecLoopName([42 x i8]* @str5) nounwind ; [#uses=0 type=i32]
  %indvar.next5 = add i7 %indvar4, 1              ; [#uses=1 type=i7]
  %tmp.1 = zext i7 %indvar4 to i64, !dbg !216     ; [#uses=2 type=i64] [debug line = 27:3]
  %AFAProcessFunctionParamBlock.addr.1 = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp.1, !dbg !216 ; [#uses=1 type=i32*] [debug line = 27:3]
  %AFAProcessFunctionParamBlock.load = load i32* %AFAProcessFunctionParamBlock.addr.1, align 4, !dbg !216 ; [#uses=1 type=i32] [debug line = 27:3]
  %p.addr.1 = getelementptr i32* %p, i64 %tmp.1, !dbg !216 ; [#uses=1 type=i32*] [debug line = 27:3]
  store i32 %AFAProcessFunctionParamBlock.load, i32* %p.addr.1, align 4, !dbg !216 ; [debug line = 27:3]
  %burstwrite.rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @.str8, i32 %burstwrite.rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  ret void, !dbg !217                             ; [debug line = 28:1]
}

; [#uses=1]
declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

; [#uses=3]
declare void @_ssdm_op_SpecInterface(...) nounwind

; [#uses=3]
declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

; [#uses=1]
declare void @_ssdm_op_SpecTopModule(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecLoopName(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecPipeline(...)

; [#uses=2]
declare i32 @_ssdm_SpecLoopFlatten(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecBurst(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecRegionBegin(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecRegionEnd(...)

; [#uses=3]
declare void @_ssdm_op_SpecBitsMap(...)

; [#uses=2]
declare i32 @_ssdm_op_SpecLoopTripCount(...)

!llvm.dbg.cu = !{!0}
!llvm.map.gv = !{}

!0 = metadata !{i32 786449, i32 0, i32 4, metadata !"C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA/proj_axi_master/solution2/.autopilot/db/AFAProcessFunction.pragma.2.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA", metadata !"clang version 3.1 ", i1 true, i1 false, metadata !"", i32 0, metadata !1, metadata !152, metadata !154, metadata !166} ; [ DW_TAG_compile_unit ]
!1 = metadata !{metadata !2}
!2 = metadata !{metadata !3, metadata !25, metadata !115, metadata !133}
!3 = metadata !{i32 786436, metadata !4, metadata !"SpectraType", metadata !5, i32 59, i64 32, i64 32, i32 0, i32 0, null, metadata !81, i32 0, i32 0} ; [ DW_TAG_enumeration_type ]
!4 = metadata !{i32 786434, null, metadata !"AFASpectra", metadata !5, i32 16, i64 19200, i64 64, i32 0, i32 0, null, metadata !6, i32 0, null, null} ; [ DW_TAG_class_type ]
!5 = metadata !{i32 786473, metadata !"proj_axi_master/src/../../include/AFASpectra.h", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA", null} ; [ DW_TAG_file_type ]
!6 = metadata !{metadata !7, metadata !12, metadata !13, metadata !14, metadata !17, metadata !20, metadata !23, metadata !24, metadata !38, metadata !40, metadata !41, metadata !43, metadata !47, metadata !53, metadata !58, metadata !59, metadata !60, metadata !61, metadata !64, metadata !65, metadata !66, metadata !67, metadata !73, metadata !76, metadata !77}
!7 = metadata !{i32 786445, metadata !4, metadata !"m_Amplitude", metadata !5, i32 368, i64 18784, i64 32, i64 0, i32 0, metadata !8} ; [ DW_TAG_member ]
!8 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 18784, i64 32, i32 0, i32 0, metadata !9, metadata !10, i32 0, i32 0} ; [ DW_TAG_array_type ]
!9 = metadata !{i32 786468, null, metadata !"float", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ]
!10 = metadata !{metadata !11}
!11 = metadata !{i32 786465, i64 0, i64 586}      ; [ DW_TAG_subrange_type ]
!12 = metadata !{i32 786445, metadata !4, metadata !"m_Min", metadata !5, i32 369, i64 32, i64 32, i64 18784, i32 0, metadata !9} ; [ DW_TAG_member ]
!13 = metadata !{i32 786445, metadata !4, metadata !"m_Max", metadata !5, i32 370, i64 32, i64 32, i64 18816, i32 0, metadata !9} ; [ DW_TAG_member ]
!14 = metadata !{i32 786445, metadata !4, metadata !"m_Index", metadata !5, i32 371, i64 32, i64 32, i64 18848, i32 0, metadata !15} ; [ DW_TAG_member ]
!15 = metadata !{i32 786454, null, metadata !"sint32_t", metadata !5, i32 8, i64 0, i64 0, i64 0, i32 0, metadata !16} ; [ DW_TAG_typedef ]
!16 = metadata !{i32 786468, null, metadata !"int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!17 = metadata !{i32 786445, metadata !4, metadata !"m_SamplesRead", metadata !5, i32 372, i64 16, i64 16, i64 18880, i32 0, metadata !18} ; [ DW_TAG_member ]
!18 = metadata !{i32 786454, null, metadata !"sint16_t", metadata !5, i32 6, i64 0, i64 0, i64 0, i32 0, metadata !19} ; [ DW_TAG_typedef ]
!19 = metadata !{i32 786468, null, metadata !"short", null, i32 0, i64 16, i64 16, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!20 = metadata !{i32 786445, metadata !4, metadata !"m_SpecObjID", metadata !5, i32 373, i64 64, i64 64, i64 18944, i32 0, metadata !21} ; [ DW_TAG_member ]
!21 = metadata !{i32 786454, null, metadata !"sint64_t", metadata !5, i32 10, i64 0, i64 0, i64 0, i32 0, metadata !22} ; [ DW_TAG_typedef ]
!22 = metadata !{i32 786468, null, metadata !"long long int", null, i32 0, i64 64, i64 64, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!23 = metadata !{i32 786445, metadata !4, metadata !"m_Type", metadata !5, i32 374, i64 32, i64 32, i64 19008, i32 0, metadata !3} ; [ DW_TAG_member ]
!24 = metadata !{i32 786445, metadata !4, metadata !"m_version", metadata !5, i32 375, i64 4, i64 4, i64 19040, i32 0, metadata !25} ; [ DW_TAG_member ]
!25 = metadata !{i32 786436, metadata !4, metadata !"SpectraVersion", metadata !5, i32 42, i64 4, i64 4, i32 0, i32 0, null, metadata !26, i32 0, i32 0} ; [ DW_TAG_enumeration_type ]
!26 = metadata !{metadata !27, metadata !28, metadata !29, metadata !30, metadata !31, metadata !32, metadata !33, metadata !34, metadata !35, metadata !36, metadata !37}
!27 = metadata !{i32 786472, metadata !"SP_VERSION_INVALID", i64 0} ; [ DW_TAG_enumerator ]
!28 = metadata !{i32 786472, metadata !"SP_ARTIFICIAL", i64 1} ; [ DW_TAG_enumerator ]
!29 = metadata !{i32 786472, metadata !"SP_CSV", i64 2} ; [ DW_TAG_enumerator ]
!30 = metadata !{i32 786472, metadata !"SP_VERSION_DR7", i64 3} ; [ DW_TAG_enumerator ]
!31 = metadata !{i32 786472, metadata !"SP_VERSION_DR8", i64 4} ; [ DW_TAG_enumerator ]
!32 = metadata !{i32 786472, metadata !"SP_VERSION_BOSS", i64 5} ; [ DW_TAG_enumerator ]
!33 = metadata !{i32 786472, metadata !"SP_VERSION_APOGEE", i64 6} ; [ DW_TAG_enumerator ]
!34 = metadata !{i32 786472, metadata !"SP_LIGHTCURVE_SDSS", i64 7} ; [ DW_TAG_enumerator ]
!35 = metadata !{i32 786472, metadata !"SP_LIGHTCURVE_RADEC", i64 8} ; [ DW_TAG_enumerator ]
!36 = metadata !{i32 786472, metadata !"SP_LIGHTCURVE_PLAIN", i64 9} ; [ DW_TAG_enumerator ]
!37 = metadata !{i32 786472, metadata !"SP_COUNT", i64 10} ; [ DW_TAG_enumerator ]
!38 = metadata !{i32 786445, metadata !4, metadata !"m_Z", metadata !5, i32 376, i64 64, i64 64, i64 19072, i32 0, metadata !39} ; [ DW_TAG_member ]
!39 = metadata !{i32 786468, null, metadata !"double", null, i32 0, i64 64, i64 64, i64 0, i32 0, i32 4} ; [ DW_TAG_base_type ]
!40 = metadata !{i32 786445, metadata !4, metadata !"m_flux", metadata !5, i32 380, i64 32, i64 32, i64 19136, i32 0, metadata !9} ; [ DW_TAG_member ]
!41 = metadata !{i32 786445, metadata !4, metadata !"m_status", metadata !5, i32 381, i64 8, i64 8, i64 19168, i32 0, metadata !42} ; [ DW_TAG_member ]
!42 = metadata !{i32 786468, null, metadata !"char", null, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!43 = metadata !{i32 786445, metadata !4, metadata !"pad", metadata !5, i32 382, i64 24, i64 8, i64 19176, i32 0, metadata !44} ; [ DW_TAG_member ]
!44 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 24, i64 8, i32 0, i32 0, metadata !42, metadata !45, i32 0, i32 0} ; [ DW_TAG_array_type ]
!45 = metadata !{metadata !46}
!46 = metadata !{i32 786465, i64 0, i64 2}        ; [ DW_TAG_subrange_type ]
!47 = metadata !{i32 786478, i32 0, metadata !4, metadata !"AFASpectra", metadata !"AFASpectra", metadata !"", metadata !5, i32 189, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 189} ; [ DW_TAG_subprogram ]
!48 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !49, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!49 = metadata !{null, metadata !50}
!50 = metadata !{i32 786447, i32 0, metadata !"", i32 0, i32 0, i64 64, i64 64, i64 0, i32 64, metadata !4} ; [ DW_TAG_pointer_type ]
!51 = metadata !{metadata !52}
!52 = metadata !{i32 786468}                      ; [ DW_TAG_base_type ]
!53 = metadata !{i32 786478, i32 0, metadata !4, metadata !"AFASpectra", metadata !"AFASpectra", metadata !"", metadata !5, i32 190, metadata !54, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 190} ; [ DW_TAG_subprogram ]
!54 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !55, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!55 = metadata !{null, metadata !50, metadata !56}
!56 = metadata !{i32 786448, null, null, null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !57} ; [ DW_TAG_reference_type ]
!57 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !4} ; [ DW_TAG_const_type ]
!58 = metadata !{i32 786478, i32 0, metadata !4, metadata !"~AFASpectra", metadata !"~AFASpectra", metadata !"", metadata !5, i32 191, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 191} ; [ DW_TAG_subprogram ]
!59 = metadata !{i32 786478, i32 0, metadata !4, metadata !"clear", metadata !"clear", metadata !"_ZN10AFASpectra5clearEv", metadata !5, i32 252, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 252} ; [ DW_TAG_subprogram ]
!60 = metadata !{i32 786478, i32 0, metadata !4, metadata !"set", metadata !"set", metadata !"_ZN10AFASpectra3setERKS_", metadata !5, i32 254, metadata !54, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 254} ; [ DW_TAG_subprogram ]
!61 = metadata !{i32 786478, i32 0, metadata !4, metadata !"randomize", metadata !"randomize", metadata !"_ZN10AFASpectra9randomizeEff", metadata !5, i32 268, metadata !62, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 268} ; [ DW_TAG_subprogram ]
!62 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !63, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!63 = metadata !{null, metadata !50, metadata !9, metadata !9}
!64 = metadata !{i32 786478, i32 0, metadata !4, metadata !"calcMinMax", metadata !"calcMinMax", metadata !"_ZN10AFASpectra10calcMinMaxEv", metadata !5, i32 343, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 343} ; [ DW_TAG_subprogram ]
!65 = metadata !{i32 786478, i32 0, metadata !4, metadata !"calculateFlux", metadata !"calculateFlux", metadata !"_ZN10AFASpectra13calculateFluxEv", metadata !5, i32 346, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 346} ; [ DW_TAG_subprogram ]
!66 = metadata !{i32 786478, i32 0, metadata !4, metadata !"normalizeByFlux", metadata !"normalizeByFlux", metadata !"_ZN10AFASpectra15normalizeByFluxEv", metadata !5, i32 352, metadata !48, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 352} ; [ DW_TAG_subprogram ]
!67 = metadata !{i32 786478, i32 0, metadata !4, metadata !"write", metadata !"write", metadata !"_ZN10AFASpectra5writeEPS_yff", metadata !5, i32 390, metadata !68, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 390} ; [ DW_TAG_subprogram ]
!68 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !69, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!69 = metadata !{null, metadata !50, metadata !70, metadata !71, metadata !9, metadata !9}
!70 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !4} ; [ DW_TAG_pointer_type ]
!71 = metadata !{i32 786454, null, metadata !"size_t", metadata !5, i32 35, i64 0, i64 0, i64 0, i32 0, metadata !72} ; [ DW_TAG_typedef ]
!72 = metadata !{i32 786468, null, metadata !"long long unsigned int", null, i32 0, i64 64, i64 64, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!73 = metadata !{i32 786478, i32 0, metadata !4, metadata !"getSDSSSpectraOffsetStart", metadata !"getSDSSSpectraOffsetStart", metadata !"_ZN10AFASpectra25getSDSSSpectraOffsetStartEv", metadata !5, i32 428, metadata !74, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 428} ; [ DW_TAG_subprogram ]
!74 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !75, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!75 = metadata !{metadata !16}
!76 = metadata !{i32 786478, i32 0, metadata !4, metadata !"getSDSSSpectraOffsetEnd", metadata !"getSDSSSpectraOffsetEnd", metadata !"_ZN10AFASpectra23getSDSSSpectraOffsetEndEv", metadata !5, i32 429, metadata !74, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 429} ; [ DW_TAG_subprogram ]
!77 = metadata !{i32 786478, i32 0, metadata !4, metadata !"setOperationRange", metadata !"setOperationRange", metadata !"_ZN10AFASpectra17setOperationRangeEb", metadata !5, i32 432, metadata !78, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 432} ; [ DW_TAG_subprogram ]
!78 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !79, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!79 = metadata !{null, metadata !80}
!80 = metadata !{i32 786468, null, metadata !"bool", null, i32 0, i64 8, i64 8, i64 0, i32 0, i32 2} ; [ DW_TAG_base_type ]
!81 = metadata !{metadata !82, metadata !83, metadata !84, metadata !85, metadata !86, metadata !87, metadata !88, metadata !89, metadata !90, metadata !91, metadata !92, metadata !93, metadata !94, metadata !95, metadata !96, metadata !97, metadata !98, metadata !99, metadata !100, metadata !101, metadata !102, metadata !103, metadata !104, metadata !105, metadata !106, metadata !107, metadata !108, metadata !109, metadata !110, metadata !111, metadata !112, metadata !113, metadata !114}
!82 = metadata !{i32 786472, metadata !"SPT_NOT_SET", i64 0} ; [ DW_TAG_enumerator ]
!83 = metadata !{i32 786472, metadata !"SPT_UNKNOWN", i64 1} ; [ DW_TAG_enumerator ]
!84 = metadata !{i32 786472, metadata !"SPT_STAR", i64 2} ; [ DW_TAG_enumerator ]
!85 = metadata !{i32 786472, metadata !"SPT_GALAXY", i64 4} ; [ DW_TAG_enumerator ]
!86 = metadata !{i32 786472, metadata !"SPT_QSO", i64 8} ; [ DW_TAG_enumerator ]
!87 = metadata !{i32 786472, metadata !"SPT_HIZ_QSO", i64 16} ; [ DW_TAG_enumerator ]
!88 = metadata !{i32 786472, metadata !"SPT_SKY", i64 32} ; [ DW_TAG_enumerator ]
!89 = metadata !{i32 786472, metadata !"SPT_STAR_LATE", i64 64} ; [ DW_TAG_enumerator ]
!90 = metadata !{i32 786472, metadata !"SPT_GAL_EM", i64 128} ; [ DW_TAG_enumerator ]
!91 = metadata !{i32 786472, metadata !"SPT_QA", i64 256} ; [ DW_TAG_enumerator ]
!92 = metadata !{i32 786472, metadata !"SPT_STAR_PN", i64 512} ; [ DW_TAG_enumerator ]
!93 = metadata !{i32 786472, metadata !"SPT_STAR_CARBON", i64 1024} ; [ DW_TAG_enumerator ]
!94 = metadata !{i32 786472, metadata !"SPT_STAR_BROWN_DWARF", i64 2048} ; [ DW_TAG_enumerator ]
!95 = metadata !{i32 786472, metadata !"SPT_STAR_SUB_DWARF", i64 4096} ; [ DW_TAG_enumerator ]
!96 = metadata !{i32 786472, metadata !"SPT_STAR_CATY_VAR", i64 8192} ; [ DW_TAG_enumerator ]
!97 = metadata !{i32 786472, metadata !"SPT_STAR_RED_DWARF", i64 16384} ; [ DW_TAG_enumerator ]
!98 = metadata !{i32 786472, metadata !"SPT_STAR_WHITE_DWARF", i64 32768} ; [ DW_TAG_enumerator ]
!99 = metadata !{i32 786472, metadata !"SPT_STAR_BHB", i64 65536} ; [ DW_TAG_enumerator ]
!100 = metadata !{i32 786472, metadata !"SPT_ROSAT_A", i64 131072} ; [ DW_TAG_enumerator ]
!101 = metadata !{i32 786472, metadata !"SPT_ROSAT_B", i64 262144} ; [ DW_TAG_enumerator ]
!102 = metadata !{i32 786472, metadata !"SPT_ROSAT_C", i64 524288} ; [ DW_TAG_enumerator ]
!103 = metadata !{i32 786472, metadata !"SPT_ROSAT_D", i64 1048576} ; [ DW_TAG_enumerator ]
!104 = metadata !{i32 786472, metadata !"SPT_SPECTROPHOTO_STD", i64 2097152} ; [ DW_TAG_enumerator ]
!105 = metadata !{i32 786472, metadata !"SPT_HOT_STD", i64 4194304} ; [ DW_TAG_enumerator ]
!106 = metadata !{i32 786472, metadata !"SPT_SERENDIPITY_BLUE", i64 8388608} ; [ DW_TAG_enumerator ]
!107 = metadata !{i32 786472, metadata !"SPT_SERENDIPITY_FIRST", i64 16777216} ; [ DW_TAG_enumerator ]
!108 = metadata !{i32 786472, metadata !"SPT_SERENDIPITY_RED", i64 33554432} ; [ DW_TAG_enumerator ]
!109 = metadata !{i32 786472, metadata !"SPT_SERENDIPITY_DISTANT", i64 67108864} ; [ DW_TAG_enumerator ]
!110 = metadata !{i32 786472, metadata !"SPT_SERENDIPITY_MANUAL", i64 134217728} ; [ DW_TAG_enumerator ]
!111 = metadata !{i32 786472, metadata !"SPT_REDDEN_STD", i64 268435456} ; [ DW_TAG_enumerator ]
!112 = metadata !{i32 786472, metadata !"SPT_BLAZAR", i64 536870912} ; [ DW_TAG_enumerator ]
!113 = metadata !{i32 786472, metadata !"SPT_QSO_BAL", i64 1073741824} ; [ DW_TAG_enumerator ]
!114 = metadata !{i32 786472, metadata !"SPT_EXOTIC", i64 2147483648} ; [ DW_TAG_enumerator ]
!115 = metadata !{i32 786436, metadata !116, metadata !"", metadata !117, i32 29, i64 3, i64 4, i32 0, i32 0, null, metadata !143, i32 0, i32 0} ; [ DW_TAG_enumeration_type ]
!116 = metadata !{i32 786434, null, metadata !"AFAParameters", metadata !117, i32 26, i64 384, i64 64, i32 0, i32 0, null, metadata !118, i32 0, null, null} ; [ DW_TAG_class_type ]
!117 = metadata !{i32 786473, metadata !"proj_axi_master/src/../../include/AFANetworkSettings.h", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA", null} ; [ DW_TAG_file_type ]
!118 = metadata !{metadata !119, metadata !120, metadata !121, metadata !122, metadata !123, metadata !124, metadata !125, metadata !126, metadata !127, metadata !128, metadata !129, metadata !130, metadata !132, metadata !138, metadata !139}
!119 = metadata !{i32 786445, metadata !116, metadata !"numSteps", metadata !117, i32 46, i64 64, i64 64, i64 0, i32 0, metadata !71} ; [ DW_TAG_member ]
!120 = metadata !{i32 786445, metadata !116, metadata !"randomSeed", metadata !117, i32 47, i64 64, i64 64, i64 64, i32 0, metadata !71} ; [ DW_TAG_member ]
!121 = metadata !{i32 786445, metadata !116, metadata !"lRateBegin", metadata !117, i32 48, i64 32, i64 32, i64 128, i32 0, metadata !9} ; [ DW_TAG_member ]
!122 = metadata !{i32 786445, metadata !116, metadata !"lRateEnd", metadata !117, i32 49, i64 32, i64 32, i64 160, i32 0, metadata !9} ; [ DW_TAG_member ]
!123 = metadata !{i32 786445, metadata !116, metadata !"radiusBegin", metadata !117, i32 50, i64 32, i64 32, i64 192, i32 0, metadata !9} ; [ DW_TAG_member ]
!124 = metadata !{i32 786445, metadata !116, metadata !"radiusEnd", metadata !117, i32 51, i64 32, i64 32, i64 224, i32 0, metadata !9} ; [ DW_TAG_member ]
!125 = metadata !{i32 786445, metadata !116, metadata !"exportSubPage", metadata !117, i32 52, i64 8, i64 8, i64 256, i32 0, metadata !80} ; [ DW_TAG_member ]
!126 = metadata !{i32 786445, metadata !116, metadata !"waitForUser", metadata !117, i32 53, i64 8, i64 8, i64 264, i32 0, metadata !80} ; [ DW_TAG_member ]
!127 = metadata !{i32 786445, metadata !116, metadata !"fullExport", metadata !117, i32 54, i64 8, i64 8, i64 272, i32 0, metadata !80} ; [ DW_TAG_member ]
!128 = metadata !{i32 786445, metadata !116, metadata !"imageBoderSize", metadata !117, i32 55, i64 32, i64 32, i64 288, i32 0, metadata !16} ; [ DW_TAG_member ]
!129 = metadata !{i32 786445, metadata !116, metadata !"iconSize", metadata !117, i32 56, i64 32, i64 32, i64 320, i32 0, metadata !16} ; [ DW_TAG_member ]
!130 = metadata !{i32 786445, metadata !116, metadata !"sstrSearchMode", metadata !117, i32 58, i64 3, i64 4, i64 352, i32 0, metadata !131} ; [ DW_TAG_member ]
!131 = metadata !{i32 786454, metadata !116, metadata !"AFANET_SETTINGS", metadata !117, i32 41, i64 0, i64 0, i64 0, i32 0, metadata !115} ; [ DW_TAG_typedef ]
!132 = metadata !{i32 786445, metadata !116, metadata !"normaliziationType", metadata !117, i32 62, i64 2, i64 2, i64 360, i32 0, metadata !133} ; [ DW_TAG_member ]
!133 = metadata !{i32 786436, metadata !4, metadata !"SpectraNormalization", metadata !5, i32 125, i64 2, i64 2, i32 0, i32 0, null, metadata !134, i32 0, i32 0} ; [ DW_TAG_enumeration_type ]
!134 = metadata !{metadata !135, metadata !136, metadata !137}
!135 = metadata !{i32 786472, metadata !"SN_NONE", i64 0} ; [ DW_TAG_enumerator ]
!136 = metadata !{i32 786472, metadata !"SN_AMPLITUDE", i64 1} ; [ DW_TAG_enumerator ]
!137 = metadata !{i32 786472, metadata !"SN_FLUX", i64 2} ; [ DW_TAG_enumerator ]
!138 = metadata !{i32 786445, metadata !116, metadata !"useBOSSWavelengthRange", metadata !117, i32 64, i64 8, i64 8, i64 368, i32 0, metadata !80} ; [ DW_TAG_member ]
!139 = metadata !{i32 786478, i32 0, metadata !116, metadata !"AFAParameters", metadata !"AFAParameters", metadata !"", metadata !117, i32 44, metadata !140, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !51, i32 44} ; [ DW_TAG_subprogram ]
!140 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !141, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!141 = metadata !{null, metadata !142, metadata !71, metadata !71, metadata !9, metadata !9, metadata !9, metadata !9}
!142 = metadata !{i32 786447, i32 0, metadata !"", i32 0, i32 0, i64 64, i64 64, i64 0, i32 64, metadata !116} ; [ DW_TAG_pointer_type ]
!143 = metadata !{metadata !144, metadata !145, metadata !146, metadata !147, metadata !148, metadata !149, metadata !150, metadata !151}
!144 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_SEARCHMODE_localfast", i64 0} ; [ DW_TAG_enumerator ]
!145 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_SEARCHMODE_local", i64 1} ; [ DW_TAG_enumerator ]
!146 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_SEARCHMODE_global", i64 2} ; [ DW_TAG_enumerator ]
!147 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_NORMALIZATION_amplitude", i64 3} ; [ DW_TAG_enumerator ]
!148 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_NORMALIZATION_flux", i64 4} ; [ DW_TAG_enumerator ]
!149 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_NORMALIZATION_none", i64 5} ; [ DW_TAG_enumerator ]
!150 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_WAVELENGTHRANGE_sdss", i64 6} ; [ DW_TAG_enumerator ]
!151 = metadata !{i32 786472, metadata !"AFANET_SETTINGS_WAVELENGTHRANGE_boss", i64 7} ; [ DW_TAG_enumerator ]
!152 = metadata !{metadata !153}
!153 = metadata !{i32 0}
!154 = metadata !{metadata !155}
!155 = metadata !{metadata !156}
!156 = metadata !{i32 786478, i32 0, metadata !157, metadata !"AFAProcessFunction", metadata !"AFAProcessFunction", metadata !"_Z18AFAProcessFunctionPVjPVvS2_", metadata !157, i32 12, metadata !158, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i32*, i8*, i8*)* @AFAProcessFunction, null, null, metadata !51, i32 16} ; [ DW_TAG_subprogram ]
!157 = metadata !{i32 786473, metadata !"proj_axi_master/src/AFAProcessFunction.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA", null} ; [ DW_TAG_file_type ]
!158 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !159, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!159 = metadata !{null, metadata !160, metadata !164, metadata !164}
!160 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !161} ; [ DW_TAG_pointer_type ]
!161 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !162} ; [ DW_TAG_volatile_type ]
!162 = metadata !{i32 786454, null, metadata !"uint32_t", metadata !157, i32 9, i64 0, i64 0, i64 0, i32 0, metadata !163} ; [ DW_TAG_typedef ]
!163 = metadata !{i32 786468, null, metadata !"unsigned int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!164 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !165} ; [ DW_TAG_pointer_type ]
!165 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, null} ; [ DW_TAG_volatile_type ]
!166 = metadata !{metadata !167}
!167 = metadata !{metadata !168, metadata !170, metadata !171, metadata !172, metadata !173, metadata !174, metadata !175, metadata !176, metadata !177, metadata !178, metadata !180, metadata !181, metadata !182, metadata !183}
!168 = metadata !{i32 786484, i32 0, metadata !4, metadata !"numSamplesSDSS", metadata !"numSamplesSDSS", metadata !"numSamplesSDSS", metadata !5, i32 19, metadata !169, i32 1, i32 1, i32 3900} ; [ DW_TAG_variable ]
!169 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !16} ; [ DW_TAG_const_type ]
!170 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveLenStartSDSS", metadata !"waveLenStartSDSS", metadata !"waveLenStartSDSS", metadata !5, i32 20, metadata !169, i32 1, i32 1, i32 3800} ; [ DW_TAG_variable ]
!171 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveLenEndSDSS", metadata !"waveLenEndSDSS", metadata !"waveLenEndSDSS", metadata !5, i32 21, metadata !169, i32 1, i32 1, i32 9200} ; [ DW_TAG_variable ]
!172 = metadata !{i32 786484, i32 0, metadata !4, metadata !"numSamplesBOSS", metadata !"numSamplesBOSS", metadata !"numSamplesBOSS", metadata !5, i32 23, metadata !169, i32 1, i32 1, i32 4700} ; [ DW_TAG_variable ]
!173 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveLenStartBOSS", metadata !"waveLenStartBOSS", metadata !"waveLenStartBOSS", metadata !5, i32 24, metadata !169, i32 1, i32 1, i32 3650} ; [ DW_TAG_variable ]
!174 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveLenEndBOSS", metadata !"waveLenEndBOSS", metadata !"waveLenEndBOSS", metadata !5, i32 25, metadata !169, i32 1, i32 1, i32 10400} ; [ DW_TAG_variable ]
!175 = metadata !{i32 786484, i32 0, metadata !4, metadata !"reductionFactor", metadata !"reductionFactor", metadata !"reductionFactor", metadata !5, i32 27, metadata !169, i32 1, i32 1, i32 8} ; [ DW_TAG_variable ]
!176 = metadata !{i32 786484, i32 0, metadata !4, metadata !"numSamples", metadata !"numSamples", metadata !"numSamples", metadata !5, i32 28, metadata !169, i32 1, i32 1, i32 587} ; [ DW_TAG_variable ]
!177 = metadata !{i32 786484, i32 0, metadata !4, metadata !"numSpectraLines", metadata !"numSpectraLines", metadata !"numSpectraLines", metadata !5, i32 29, metadata !169, i32 1, i32 1, i32 44} ; [ DW_TAG_variable ]
!178 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveBeginDst", metadata !"waveBeginDst", metadata !"_ZN10AFASpectra12waveBeginDstE", metadata !5, i32 30, metadata !179, i32 0, i32 1, null} ; [ DW_TAG_variable ]
!179 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !9} ; [ DW_TAG_const_type ]
!180 = metadata !{i32 786484, i32 0, metadata !4, metadata !"waveEndDst", metadata !"waveEndDst", metadata !"_ZN10AFASpectra10waveEndDstE", metadata !5, i32 31, metadata !179, i32 0, i32 1, null} ; [ DW_TAG_variable ]
!181 = metadata !{i32 786484, i32 0, metadata !4, metadata !"pixelStart", metadata !"pixelStart", metadata !"_ZN10AFASpectra10pixelStartE", metadata !5, i32 33, metadata !16, i32 0, i32 1, null} ; [ DW_TAG_variable ]
!182 = metadata !{i32 786484, i32 0, metadata !4, metadata !"pixelEnd", metadata !"pixelEnd", metadata !"_ZN10AFASpectra8pixelEndE", metadata !5, i32 34, metadata !16, i32 0, i32 1, null} ; [ DW_TAG_variable ]
!183 = metadata !{i32 786484, i32 0, metadata !116, metadata !"defaultParameters", metadata !"defaultParameters", metadata !"_ZN13AFAParameters17defaultParametersE", metadata !117, i32 67, metadata !116, i32 0, i32 1, null} ; [ DW_TAG_variable ]
!184 = metadata !{metadata !185}
!185 = metadata !{i32 0, i32 31, metadata !186}
!186 = metadata !{metadata !187}
!187 = metadata !{metadata !"p", metadata !188, metadata !"unsigned int", i32 0, i32 31}
!188 = metadata !{metadata !189}
!189 = metadata !{i32 0, i32 63, i32 1}
!190 = metadata !{metadata !191}
!191 = metadata !{i32 0, i32 7, metadata !192}
!192 = metadata !{metadata !193}
!193 = metadata !{metadata !"srcNet", metadata !194, metadata !"void", i32 0, i32 7}
!194 = metadata !{metadata !195}
!195 = metadata !{i32 0, i32 255, i32 1}
!196 = metadata !{metadata !197}
!197 = metadata !{i32 0, i32 7, metadata !198}
!198 = metadata !{metadata !199}
!199 = metadata !{metadata !"dstNet", metadata !194, metadata !"void", i32 0, i32 7}
!200 = metadata !{i32 786689, metadata !156, metadata !"p", metadata !157, i32 16777229, metadata !160, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!201 = metadata !{i32 13, i32 24, metadata !156, null}
!202 = metadata !{i32 786689, metadata !156, metadata !"srcNet", metadata !157, i32 33554446, metadata !164, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!203 = metadata !{i32 14, i32 20, metadata !156, null}
!204 = metadata !{i32 786689, metadata !156, metadata !"dstNet", metadata !157, i32 50331663, metadata !164, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!205 = metadata !{i32 15, i32 20, metadata !156, null}
!206 = metadata !{i32 17, i32 1, metadata !207, null}
!207 = metadata !{i32 786443, metadata !156, i32 16, i32 1, metadata !157, i32 0} ; [ DW_TAG_lexical_block ]
!208 = metadata !{i32 18, i32 1, metadata !207, null}
!209 = metadata !{i32 19, i32 1, metadata !207, null}
!210 = metadata !{i32 786688, metadata !207, metadata !"AFAProcessFunctionParamBlock", metadata !157, i32 23, metadata !211, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!211 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 2048, i64 32, i32 0, i32 0, metadata !162, metadata !212, i32 0, i32 0} ; [ DW_TAG_array_type ]
!212 = metadata !{metadata !213}
!213 = metadata !{i32 786465, i64 0, i64 63}      ; [ DW_TAG_subrange_type ]
!214 = metadata !{i32 23, i32 12, metadata !207, null}
!215 = metadata !{i32 26, i32 3, metadata !207, null}
!216 = metadata !{i32 27, i32 3, metadata !207, null}
!217 = metadata !{i32 28, i32 1, metadata !207, null}
