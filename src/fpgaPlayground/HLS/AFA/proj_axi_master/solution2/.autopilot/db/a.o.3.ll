; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA/proj_axi_master/solution2/.autopilot/db/a.o.3.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@p_str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1 ; [#uses=3 type=[6 x i8]*]
@p_str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1 ; [#uses=9 type=[1 x i8]*]
@str = internal constant [19 x i8] c"AFAProcessFunction\00" ; [#uses=1 type=[19 x i8]*]
@str1 = internal constant [38 x i8] c"memcpy.AFAProcessFunctionParamBlock.p\00" ; [#uses=1 type=[38 x i8]*]
@str2 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@p_str4 = internal constant [17 x i8] c"burstread.region\00" ; [#uses=2 type=[17 x i8]*]
@str5 = internal constant [42 x i8] c"memcpy.p.AFAProcessFunctionParamBlock.gep\00" ; [#uses=1 type=[42 x i8]*]
@str6 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@p_str8 = internal constant [18 x i8] c"burstwrite.region\00" ; [#uses=2 type=[18 x i8]*]

; [#uses=0]
define void @AFAProcessFunction(i32* %p, i8* %srcNet, i8* %dstNet) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %p) nounwind, !map !0
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %srcNet) nounwind, !map !6
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %dstNet) nounwind, !map !12
  call void (...)* @_ssdm_op_SpecTopModule([19 x i8]* @str) nounwind
  %AFAProcessFunctionParamBlock = alloca [64 x i32], align 16 ; [#uses=2 type=[64 x i32]*]
  call void @llvm.dbg.value(metadata !{i32* %p}, i64 0, metadata !16), !dbg !29 ; [debug line = 13:24] [debug variable = p]
  call void @llvm.dbg.value(metadata !{i8* %srcNet}, i64 0, metadata !30), !dbg !31 ; [debug line = 14:20] [debug variable = srcNet]
  call void @llvm.dbg.value(metadata !{i8* %dstNet}, i64 0, metadata !32), !dbg !33 ; [debug line = 15:20] [debug variable = dstNet]
  call void (...)* @_ssdm_op_SpecInterface(i32* %p, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 64, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind, !dbg !34 ; [debug line = 17:1]
  call void (...)* @_ssdm_op_SpecInterface(i8* %srcNet, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind, !dbg !36 ; [debug line = 18:1]
  call void (...)* @_ssdm_op_SpecInterface(i8* %dstNet, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind, !dbg !37 ; [debug line = 19:1]
  call void @llvm.dbg.declare(metadata !{[64 x i32]* %AFAProcessFunctionParamBlock}, metadata !38), !dbg !42 ; [debug line = 23:12] [debug variable = AFAProcessFunctionParamBlock]
  %p_rd_req = call i1 @_ssdm_op_ReadReq.m_axi.i32P(i32* %p, i32 64) nounwind, !dbg !43 ; [#uses=0 type=i1] [debug line = 26:3]
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i7 [ 0, %0 ], [ %indvar_next, %burst.rd.body ] ; [#uses=3 type=i7]
  %exitcond = icmp eq i7 %indvar, -64             ; [#uses=1 type=i1]
  %empty = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind ; [#uses=0 type=i32]
  %indvar_next = add i7 %indvar, 1                ; [#uses=1 type=i7]
  br i1 %exitcond, label %burst.wr.header.preheader, label %burst.rd.body

burst.wr.header.preheader:                        ; preds = %burst.rd.header
  %p_wr_req = call i1 @_ssdm_op_WriteReq.m_axi.i32P(i32* %p, i32 64) nounwind, !dbg !44 ; [#uses=0 type=i1] [debug line = 27:3]
  br label %burst.wr.header

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str4) nounwind ; [#uses=1 type=i32]
  %empty_3 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind ; [#uses=0 type=i32]
  %empty_4 = call i32 (...)* @_ssdm_op_SpecLoopName([38 x i8]* @str1) nounwind ; [#uses=0 type=i32]
  %tmp = zext i7 %indvar to i64, !dbg !43         ; [#uses=1 type=i64] [debug line = 26:3]
  %p_read = call i32 @_ssdm_op_Read.m_axi.i32P(i32* %p) nounwind, !dbg !43 ; [#uses=1 type=i32] [debug line = 26:3]
  %AFAProcessFunctionParamBlock_a = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp, !dbg !43 ; [#uses=1 type=i32*] [debug line = 26:3]
  store i32 %p_read, i32* %AFAProcessFunctionParamBlock_a, align 4, !dbg !43 ; [debug line = 26:3]
  %burstread_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str4, i32 %burstread_rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.rd.header

burst.wr.header:                                  ; preds = %burst.wr.body, %burst.wr.header.preheader
  %indvar4 = phi i7 [ %indvar_next5, %burst.wr.body ], [ 0, %burst.wr.header.preheader ] ; [#uses=3 type=i7]
  %exitcond6 = icmp eq i7 %indvar4, -64           ; [#uses=1 type=i1]
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind ; [#uses=0 type=i32]
  %indvar_next5 = add i7 %indvar4, 1              ; [#uses=1 type=i7]
  br i1 %exitcond6, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str8) nounwind ; [#uses=1 type=i32]
  %empty_6 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind ; [#uses=0 type=i32]
  %empty_7 = call i32 (...)* @_ssdm_op_SpecLoopName([42 x i8]* @str5) nounwind ; [#uses=0 type=i32]
  %tmp_1 = zext i7 %indvar4 to i64, !dbg !44      ; [#uses=1 type=i64] [debug line = 27:3]
  %AFAProcessFunctionParamBlock_a_1 = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp_1, !dbg !44 ; [#uses=1 type=i32*] [debug line = 27:3]
  %AFAProcessFunctionParamBlock_l = load i32* %AFAProcessFunctionParamBlock_a_1, align 4, !dbg !44 ; [#uses=1 type=i32] [debug line = 27:3]
  call void @_ssdm_op_Write.m_axi.i32P(i32* %p, i32 %AFAProcessFunctionParamBlock_l, i4 -1) nounwind, !dbg !44 ; [debug line = 27:3]
  %burstwrite_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str8, i32 %burstwrite_rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  %p_wr_resp = call i1 @_ssdm_op_WriteResp.m_axi.i32P(i32* %p) nounwind, !dbg !44 ; [#uses=0 type=i1] [debug line = 27:3]
  ret void, !dbg !45                              ; [debug line = 28:1]
}

; [#uses=1]
declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

; [#uses=3]
define weak void @_ssdm_op_SpecInterface(...) nounwind {
entry:
  ret void
}

; [#uses=3]
declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

; [#uses=1]
define weak void @_ssdm_op_SpecTopModule(...) {
entry:
  ret void
}

; [#uses=2]
define weak i32 @_ssdm_op_SpecLoopName(...) {
entry:
  ret i32 0
}

; [#uses=2]
define weak i32 @_ssdm_op_SpecPipeline(...) {
entry:
  ret i32 0
}

; [#uses=2]
define weak i32 @_ssdm_op_SpecRegionBegin(...) {
entry:
  ret i32 0
}

; [#uses=2]
define weak i32 @_ssdm_op_SpecRegionEnd(...) {
entry:
  ret i32 0
}

; [#uses=3]
define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

; [#uses=2]
define weak i32 @_ssdm_op_SpecLoopTripCount(...) {
entry:
  ret i32 0
}

; [#uses=1]
define weak i1 @_ssdm_op_ReadReq.m_axi.i32P(i32*, i32) {
entry:
  ret i1 true
}

; [#uses=1]
define weak i32 @_ssdm_op_Read.m_axi.i32P(i32*) {
entry:
  %empty = load i32* %0                           ; [#uses=1 type=i32]
  ret i32 %empty
}

; [#uses=1]
define weak i1 @_ssdm_op_WriteReq.m_axi.i32P(i32*, i32) {
entry:
  ret i1 true
}

; [#uses=1]
define weak void @_ssdm_op_Write.m_axi.i32P(i32*, i32, i4) {
entry:
  ret void
}

; [#uses=1]
define weak i1 @_ssdm_op_WriteResp.m_axi.i32P(i32*) {
entry:
  ret i1 true
}

!llvm.map.gv = !{}

!0 = metadata !{metadata !1}
!1 = metadata !{i32 0, i32 31, metadata !2}
!2 = metadata !{metadata !3}
!3 = metadata !{metadata !"p", metadata !4, metadata !"unsigned int", i32 0, i32 31}
!4 = metadata !{metadata !5}
!5 = metadata !{i32 0, i32 63, i32 1}
!6 = metadata !{metadata !7}
!7 = metadata !{i32 0, i32 7, metadata !8}
!8 = metadata !{metadata !9}
!9 = metadata !{metadata !"srcNet", metadata !10, metadata !"void", i32 0, i32 7}
!10 = metadata !{metadata !11}
!11 = metadata !{i32 0, i32 255, i32 1}
!12 = metadata !{metadata !13}
!13 = metadata !{i32 0, i32 7, metadata !14}
!14 = metadata !{metadata !15}
!15 = metadata !{metadata !"dstNet", metadata !10, metadata !"void", i32 0, i32 7}
!16 = metadata !{i32 786689, metadata !17, metadata !"p", metadata !18, i32 16777229, metadata !21, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!17 = metadata !{i32 786478, i32 0, metadata !18, metadata !"AFAProcessFunction", metadata !"AFAProcessFunction", metadata !"_Z18AFAProcessFunctionPVjPVvS2_", metadata !18, i32 12, metadata !19, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i32*, i8*, i8*)* @AFAProcessFunction, null, null, metadata !27, i32 16} ; [ DW_TAG_subprogram ]
!18 = metadata !{i32 786473, metadata !"proj_axi_master/src/AFAProcessFunction.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA", null} ; [ DW_TAG_file_type ]
!19 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !20, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!20 = metadata !{null, metadata !21, metadata !25, metadata !25}
!21 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !22} ; [ DW_TAG_pointer_type ]
!22 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !23} ; [ DW_TAG_volatile_type ]
!23 = metadata !{i32 786454, null, metadata !"uint32_t", metadata !18, i32 9, i64 0, i64 0, i64 0, i32 0, metadata !24} ; [ DW_TAG_typedef ]
!24 = metadata !{i32 786468, null, metadata !"unsigned int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!25 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !26} ; [ DW_TAG_pointer_type ]
!26 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, null} ; [ DW_TAG_volatile_type ]
!27 = metadata !{metadata !28}
!28 = metadata !{i32 786468}                      ; [ DW_TAG_base_type ]
!29 = metadata !{i32 13, i32 24, metadata !17, null}
!30 = metadata !{i32 786689, metadata !17, metadata !"srcNet", metadata !18, i32 33554446, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!31 = metadata !{i32 14, i32 20, metadata !17, null}
!32 = metadata !{i32 786689, metadata !17, metadata !"dstNet", metadata !18, i32 50331663, metadata !25, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!33 = metadata !{i32 15, i32 20, metadata !17, null}
!34 = metadata !{i32 17, i32 1, metadata !35, null}
!35 = metadata !{i32 786443, metadata !17, i32 16, i32 1, metadata !18, i32 0} ; [ DW_TAG_lexical_block ]
!36 = metadata !{i32 18, i32 1, metadata !35, null}
!37 = metadata !{i32 19, i32 1, metadata !35, null}
!38 = metadata !{i32 786688, metadata !35, metadata !"AFAProcessFunctionParamBlock", metadata !18, i32 23, metadata !39, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!39 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 2048, i64 32, i32 0, i32 0, metadata !23, metadata !40, i32 0, i32 0} ; [ DW_TAG_array_type ]
!40 = metadata !{metadata !41}
!41 = metadata !{i32 786465, i64 0, i64 63}       ; [ DW_TAG_subrange_type ]
!42 = metadata !{i32 23, i32 12, metadata !35, null}
!43 = metadata !{i32 26, i32 3, metadata !35, null}
!44 = metadata !{i32 27, i32 3, metadata !35, null}
!45 = metadata !{i32 28, i32 1, metadata !35, null}
