; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master/proj_axi_master/solution1/.autopilot/db/a.o.2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@p_str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1
@p_str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str = internal constant [8 x i8] c"example\00"
@str1 = internal constant [14 x i8] c"memcpy.buff.a\00"
@str2 = internal constant [1 x i8] zeroinitializer
@p_str4 = internal constant [17 x i8] c"burstread.region\00"
@str5 = internal constant [18 x i8] c"memcpy.a.buff.gep\00"
@str6 = internal constant [1 x i8] zeroinitializer
@p_str8 = internal constant [18 x i8] c"burstwrite.region\00"

define void @example(i32* %a) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %a) nounwind, !map !0
  call void (...)* @_ssdm_op_SpecTopModule([8 x i8]* @str) nounwind
  %buff = alloca [50 x i32], align 16
  call void (...)* @_ssdm_op_SpecInterface(i32* %a, [6 x i8]* @p_str, i32 0, i32 0, i32 0, i32 50, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1) nounwind
  %p_rd_req = call i1 @_ssdm_op_ReadReq.m_axi.i32P(i32* %a, i32 50) nounwind
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i6 [ 0, %0 ], [ %indvar_next, %burst.rd.body ]
  %exitcond1 = icmp eq i6 %indvar, -14
  %empty = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind
  %indvar_next = add i6 %indvar, 1
  br i1 %exitcond1, label %burst.rd.end, label %burst.rd.body

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str4) nounwind
  %empty_3 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind
  %empty_4 = call i32 (...)* @_ssdm_op_SpecLoopName([14 x i8]* @str1) nounwind
  %tmp = zext i6 %indvar to i64
  %a_read = call i32 @_ssdm_op_Read.m_axi.i32P(i32* %a) nounwind
  %buff_addr = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp
  store i32 %a_read, i32* %buff_addr, align 4
  %burstread_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str4, i32 %burstread_rbegin) nounwind
  br label %burst.rd.header

burst.rd.end:                                     ; preds = %burst.rd.header, %1
  %i = phi i6 [ %i_1, %1 ], [ 0, %burst.rd.header ]
  %exitcond = icmp eq i6 %i, -14
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind
  %i_1 = add i6 %i, 1
  br i1 %exitcond, label %burst.wr.header.preheader, label %1

burst.wr.header.preheader:                        ; preds = %burst.rd.end
  %p_wr_req = call i1 @_ssdm_op_WriteReq.m_axi.i32P(i32* %a, i32 50) nounwind
  br label %burst.wr.header

; <label>:1                                       ; preds = %burst.rd.end
  %tmp_1 = zext i6 %i to i64
  %buff_addr_1 = getelementptr inbounds [50 x i32]* %buff, i64 0, i64 %tmp_1
  %buff_load = load i32* %buff_addr_1, align 4
  %tmp_2 = add nsw i32 %buff_load, 100
  store i32 %tmp_2, i32* %buff_addr_1, align 4
  br label %burst.rd.end

burst.wr.header:                                  ; preds = %burst.wr.header.preheader, %burst.wr.body
  %indvar5 = phi i6 [ %indvar_next6, %burst.wr.body ], [ 0, %burst.wr.header.preheader ]
  %exitcond7 = icmp eq i6 %indvar5, -14
  %empty_6 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 50, i64 50, i64 50) nounwind
  %indvar_next6 = add i6 %indvar5, 1
  br i1 %exitcond7, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite_rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str8) nounwind
  %empty_7 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind
  %empty_8 = call i32 (...)* @_ssdm_op_SpecLoopName([18 x i8]* @str5) nounwind
  %tmp_4 = zext i6 %indvar5 to i64
  %buff_addr_2 = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp_4
  %buff_load_1 = load i32* %buff_addr_2, align 4
  call void @_ssdm_op_Write.m_axi.i32P(i32* %a, i32 %buff_load_1, i4 -1) nounwind
  %burstwrite_rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str8, i32 %burstwrite_rbegin) nounwind
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  %p_wr_resp = call i1 @_ssdm_op_WriteResp.m_axi.i32P(i32* %a) nounwind
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
!3 = metadata !{metadata !"a", metadata !4, metadata !"int", i32 0, i32 31}
!4 = metadata !{metadata !5}
!5 = metadata !{i32 0, i32 49, i32 1}
