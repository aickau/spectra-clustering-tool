; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/AFA2/AFA/proj_axi_master/solution2/.autopilot/db/a.o.2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@p_str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1
@p_str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str = internal constant [19 x i8] c"AFAProcessFunction\00"
@str1 = internal constant [38 x i8] c"memcpy.AFAProcessFunctionParamBlock.p\00"
@str2 = internal constant [1 x i8] zeroinitializer
@p_str4 = internal constant [17 x i8] c"burstread.region\00"
@str5 = internal constant [42 x i8] c"memcpy.p.AFAProcessFunctionParamBlock.gep\00"
@str6 = internal constant [1 x i8] zeroinitializer
@p_str8 = internal constant [18 x i8] c"burstwrite.region\00"

define void @AFAProcessFunction(i32* %p, i8* %srcNet, i8* %dstNet) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %p) nounwind, !map !0
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %srcNet) nounwind, !map !6
  call void (...)* @_ssdm_op_SpecBitsMap(i8* %dstNet) nounwind, !map !12
  call void (...)* @_ssdm_op_SpecTopModule([19 x i8]* @str) nounwind
  %AFAProcessFunctionParamBlock = alloca [64 x i32], align 16
  call void (...)* @_ssdm_op_SpecInterface(i32* %p, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 64, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind
  call void (...)* @_ssdm_op_SpecInterface(i8* %srcNet, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind
  call void (...)* @_ssdm_op_SpecInterface(i8* %dstNet, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 256, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind
  %p_rd_req = call i1 @_ssdm_op_ReadReq.m_axi.i32P(i32* %p, i32 64) nounwind
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i7 [ 0, %0 ], [ %indvar_next, %burst.rd.body ]
  %exitcond = icmp eq i7 %indvar, -64
  %empty = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind
  %indvar_next = add i7 %indvar, 1
  br i1 %exitcond, label %burst.wr.header.preheader, label %burst.rd.body

burst.wr.header.preheader:                        ; preds = %burst.rd.header
  %p_wr_req = call i1 @_ssdm_op_WriteReq.m_axi.i32P(i32* %p, i32 64) nounwind
  br label %burst.wr.header

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str4) nounwind
  %empty_3 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind
  %empty_4 = call i32 (...)* @_ssdm_op_SpecLoopName([38 x i8]* @str1) nounwind
  %tmp = zext i7 %indvar to i64
  %p_read = call i32 @_ssdm_op_Read.m_axi.i32P(i32* %p) nounwind
  %AFAProcessFunctionParamBlock_a = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp
  store i32 %p_read, i32* %AFAProcessFunctionParamBlock_a, align 4
  %burstread_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str4, i32 %burstread_rbegin) nounwind
  br label %burst.rd.header

burst.wr.header:                                  ; preds = %burst.wr.header.preheader, %burst.wr.body
  %indvar4 = phi i7 [ %indvar_next5, %burst.wr.body ], [ 0, %burst.wr.header.preheader ]
  %exitcond6 = icmp eq i7 %indvar4, -64
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 64, i64 64, i64 64) nounwind
  %indvar_next5 = add i7 %indvar4, 1
  br i1 %exitcond6, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str8) nounwind
  %empty_6 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind
  %empty_7 = call i32 (...)* @_ssdm_op_SpecLoopName([42 x i8]* @str5) nounwind
  %tmp_1 = zext i7 %indvar4 to i64
  %AFAProcessFunctionParamBlock_a_1 = getelementptr [64 x i32]* %AFAProcessFunctionParamBlock, i64 0, i64 %tmp_1
  %AFAProcessFunctionParamBlock_l = load i32* %AFAProcessFunctionParamBlock_a_1, align 4
  call void @_ssdm_op_Write.m_axi.i32P(i32* %p, i32 %AFAProcessFunctionParamBlock_l, i4 -1) nounwind
  %burstwrite_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str8, i32 %burstwrite_rbegin) nounwind
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  %p_wr_resp = call i1 @_ssdm_op_WriteResp.m_axi.i32P(i32* %p) nounwind
  ret void
}

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define weak void @_ssdm_op_SpecInterface(...) nounwind {
entry:
  ret void
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

define weak void @_ssdm_op_SpecTopModule(...) {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecLoopName(...) {
entry:
  ret i32 0
}

define weak i32 @_ssdm_op_SpecPipeline(...) {
entry:
  ret i32 0
}

define weak i32 @_ssdm_op_SpecRegionBegin(...) {
entry:
  ret i32 0
}

define weak i32 @_ssdm_op_SpecRegionEnd(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecLoopTripCount(...) {
entry:
  ret i32 0
}

define weak i1 @_ssdm_op_ReadReq.m_axi.i32P(i32*, i32) {
entry:
  ret i1 true
}

define weak i32 @_ssdm_op_Read.m_axi.i32P(i32*) {
entry:
  %empty = load i32* %0
  ret i32 %empty
}

define weak i1 @_ssdm_op_WriteReq.m_axi.i32P(i32*, i32) {
entry:
  ret i1 true
}

define weak void @_ssdm_op_Write.m_axi.i32P(i32*, i32, i4) {
entry:
  ret void
}

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
