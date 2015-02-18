; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master/proj_axi_master/solution1/.autopilot/db/a.o.3.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@p_str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1 ; [#uses=1 type=[6 x i8]*]
@p_str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1 ; [#uses=3 type=[1 x i8]*]
@str = internal constant [8 x i8] c"example\00"   ; [#uses=1 type=[8 x i8]*]
@str1 = internal constant [14 x i8] c"memcpy.buff.a\00" ; [#uses=1 type=[14 x i8]*]
@str2 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@p_str4 = internal constant [17 x i8] c"burstread.region\00" ; [#uses=2 type=[17 x i8]*]
@str5 = internal constant [18 x i8] c"memcpy.a.buff.gep\00" ; [#uses=1 type=[18 x i8]*]
@str6 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@p_str8 = internal constant [18 x i8] c"burstwrite.region\00" ; [#uses=2 type=[18 x i8]*]

; [#uses=0]
define void @example(i32* %a) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %a) nounwind, !map !0
  call void (...)* @_ssdm_op_SpecTopModule([8 x i8]* @str) nounwind
  %buff = alloca [50 x i32], align 16             ; [#uses=3 type=[50 x i32]*]
  call void @llvm.dbg.value(metadata !{i32* %a}, i64 0, metadata !6), !dbg !16 ; [debug line = 99:28] [debug variable = a]
  call void (...)* @_ssdm_op_SpecInterface(i32* %a, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 50, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind, !dbg !17 ; [debug line = 101:1]
  call void @llvm.dbg.declare(metadata !{[50 x i32]* %buff}, metadata !19), !dbg !23 ; [debug line = 104:7] [debug variable = buff]
  %p_rd_req = call i1 @_ssdm_op_ReadReq.m_axi.i32P(i32* %a, i32 50) nounwind, !dbg !24 ; [#uses=0 type=i1] [debug line = 109:3]
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i6 [ 0, %0 ], [ %indvar_next, %burst.rd.body ] ; [#uses=3 type=i6]
  %exitcond1 = icmp eq i6 %indvar, -14            ; [#uses=1 type=i1]
  %empty = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind ; [#uses=0 type=i32]
  %indvar_next = add i6 %indvar, 1                ; [#uses=1 type=i6]
  br i1 %exitcond1, label %burst.rd.end, label %burst.rd.body

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str4) nounwind ; [#uses=1 type=i32]
  %empty_3 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind ; [#uses=0 type=i32]
  %empty_4 = call i32 (...)* @_ssdm_op_SpecLoopName([14 x i8]* @str1) nounwind ; [#uses=0 type=i32]
  %tmp = zext i6 %indvar to i64, !dbg !24         ; [#uses=1 type=i64] [debug line = 109:3]
  %a_read = call i32 @_ssdm_op_Read.m_axi.i32P(i32* %a) nounwind, !dbg !24 ; [#uses=1 type=i32] [debug line = 109:3]
  %buff_addr = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp, !dbg !24 ; [#uses=1 type=i32*] [debug line = 109:3]
  store i32 %a_read, i32* %buff_addr, align 4, !dbg !24 ; [debug line = 109:3]
  %burstread_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str4, i32 %burstread_rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.rd.header

burst.rd.end:                                     ; preds = %1, %burst.rd.header
  %i = phi i6 [ %i_1, %1 ], [ 0, %burst.rd.header ] ; [#uses=3 type=i6]
  %exitcond = icmp eq i6 %i, -14, !dbg !25        ; [#uses=1 type=i1] [debug line = 111:7]
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind ; [#uses=0 type=i32]
  %i_1 = add i6 %i, 1, !dbg !27                   ; [#uses=1 type=i6] [debug line = 111:20]
  br i1 %exitcond, label %burst.wr.header.preheader, label %1, !dbg !25 ; [debug line = 111:7]

burst.wr.header.preheader:                        ; preds = %burst.rd.end
  %p_wr_req = call i1 @_ssdm_op_WriteReq.m_axi.i32P(i32* %a, i32 50) nounwind, !dbg !28 ; [#uses=0 type=i1] [debug line = 115:3]
  br label %burst.wr.header

; <label>:1                                       ; preds = %burst.rd.end
  %tmp_1 = zext i6 %i to i64, !dbg !29            ; [#uses=1 type=i64] [debug line = 112:5]
  %buff_addr_1 = getelementptr inbounds [50 x i32]* %buff, i64 0, i64 %tmp_1, !dbg !29 ; [#uses=2 type=i32*] [debug line = 112:5]
  %buff_load = load i32* %buff_addr_1, align 4, !dbg !29 ; [#uses=1 type=i32] [debug line = 112:5]
  %tmp_2 = add nsw i32 %buff_load, 100, !dbg !29  ; [#uses=1 type=i32] [debug line = 112:5]
  store i32 %tmp_2, i32* %buff_addr_1, align 4, !dbg !29 ; [debug line = 112:5]
  call void @llvm.dbg.value(metadata !{i6 %i_1}, i64 0, metadata !31), !dbg !27 ; [debug line = 111:20] [debug variable = i]
  br label %burst.rd.end, !dbg !27                ; [debug line = 111:20]

burst.wr.header:                                  ; preds = %burst.wr.body, %burst.wr.header.preheader
  %indvar5 = phi i6 [ %indvar_next6, %burst.wr.body ], [ 0, %burst.wr.header.preheader ] ; [#uses=3 type=i6]
  %exitcond7 = icmp eq i6 %indvar5, -14           ; [#uses=1 type=i1]
  %empty_6 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind ; [#uses=0 type=i32]
  %indvar_next6 = add i6 %indvar5, 1              ; [#uses=1 type=i6]
  br i1 %exitcond7, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str8) nounwind ; [#uses=1 type=i32]
  %empty_7 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind ; [#uses=0 type=i32]
  %empty_8 = call i32 (...)* @_ssdm_op_SpecLoopName([18 x i8]* @str5) nounwind ; [#uses=0 type=i32]
  %tmp_4 = zext i6 %indvar5 to i64, !dbg !28      ; [#uses=1 type=i64] [debug line = 115:3]
  %buff_addr_2 = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp_4, !dbg !28 ; [#uses=1 type=i32*] [debug line = 115:3]
  %buff_load_1 = load i32* %buff_addr_2, align 4, !dbg !28 ; [#uses=1 type=i32] [debug line = 115:3]
  call void @_ssdm_op_Write.m_axi.i32P(i32* %a, i32 %buff_load_1, i4 -1) nounwind, !dbg !28 ; [debug line = 115:3]
  %burstwrite_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str8, i32 %burstwrite_rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  %p_wr_resp = call i1 @_ssdm_op_WriteResp.m_axi.i32P(i32* %a) nounwind, !dbg !28 ; [#uses=0 type=i1] [debug line = 115:3]
  ret void, !dbg !32                              ; [debug line = 116:1]
}

; [#uses=1]
declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

; [#uses=1]
define weak void @_ssdm_op_SpecInterface(...) nounwind {
entry:
  ret void
}

; [#uses=2]
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

; [#uses=1]
define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

; [#uses=3]
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
!3 = metadata !{metadata !"a", metadata !4, metadata !"int", i32 0, i32 31}
!4 = metadata !{metadata !5}
!5 = metadata !{i32 0, i32 49, i32 1}
!6 = metadata !{i32 786689, metadata !7, metadata !"a", metadata !8, i32 16777315, metadata !11, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!7 = metadata !{i32 786478, i32 0, metadata !8, metadata !"example", metadata !"example", metadata !"_Z7examplePVi", metadata !8, i32 99, metadata !9, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i32*)* @example, null, null, metadata !14, i32 99} ; [ DW_TAG_subprogram ]
!8 = metadata !{i32 786473, metadata !"example.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master", null} ; [ DW_TAG_file_type ]
!9 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !10, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!10 = metadata !{null, metadata !11}
!11 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !12} ; [ DW_TAG_pointer_type ]
!12 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !13} ; [ DW_TAG_volatile_type ]
!13 = metadata !{i32 786468, null, metadata !"int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!14 = metadata !{metadata !15}
!15 = metadata !{i32 786468}                      ; [ DW_TAG_base_type ]
!16 = metadata !{i32 99, i32 28, metadata !7, null}
!17 = metadata !{i32 101, i32 1, metadata !18, null}
!18 = metadata !{i32 786443, metadata !7, i32 99, i32 30, metadata !8, i32 0} ; [ DW_TAG_lexical_block ]
!19 = metadata !{i32 786688, metadata !18, metadata !"buff", metadata !8, i32 104, metadata !20, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!20 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 1600, i64 32, i32 0, i32 0, metadata !13, metadata !21, i32 0, i32 0} ; [ DW_TAG_array_type ]
!21 = metadata !{metadata !22}
!22 = metadata !{i32 786465, i64 0, i64 49}       ; [ DW_TAG_subrange_type ]
!23 = metadata !{i32 104, i32 7, metadata !18, null}
!24 = metadata !{i32 109, i32 3, metadata !18, null}
!25 = metadata !{i32 111, i32 7, metadata !26, null}
!26 = metadata !{i32 786443, metadata !18, i32 111, i32 3, metadata !8, i32 1} ; [ DW_TAG_lexical_block ]
!27 = metadata !{i32 111, i32 20, metadata !26, null}
!28 = metadata !{i32 115, i32 3, metadata !18, null}
!29 = metadata !{i32 112, i32 5, metadata !30, null}
!30 = metadata !{i32 786443, metadata !26, i32 111, i32 24, metadata !8, i32 2} ; [ DW_TAG_lexical_block ]
!31 = metadata !{i32 786688, metadata !18, metadata !"i", metadata !8, i32 103, metadata !13, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!32 = metadata !{i32 116, i32 1, metadata !18, null}
