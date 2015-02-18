; ModuleID = 'C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master/proj_axi_master/solution1/.autopilot/db/a.g.1.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-w64-mingw32"

@.str = private unnamed_addr constant [6 x i8] c"m_axi\00", align 1 ; [#uses=1 type=[6 x i8]*]
@.str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1 ; [#uses=1 type=[1 x i8]*]
@str = internal constant [8 x i8] c"example\00"   ; [#uses=1 type=[8 x i8]*]
@str1 = internal constant [14 x i8] c"memcpy.buff.a\00" ; [#uses=1 type=[14 x i8]*]
@str2 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@str3 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@.str4 = internal constant [17 x i8] c"burstread.region\00" ; [#uses=2 type=[17 x i8]*]
@str5 = internal constant [18 x i8] c"memcpy.a.buff.gep\00" ; [#uses=1 type=[18 x i8]*]
@str6 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@str7 = internal constant [1 x i8] zeroinitializer ; [#uses=1 type=[1 x i8]*]
@.str8 = internal constant [18 x i8] c"burstwrite.region\00" ; [#uses=2 type=[18 x i8]*]

; [#uses=0]
define void @example(i32* %a) nounwind uwtable {
  call void (...)* @_ssdm_op_SpecTopModule([8 x i8]* @str) nounwind
  %buff = alloca [50 x i32], align 16             ; [#uses=3 type=[50 x i32]*]
  call void @llvm.dbg.value(metadata !{i32* %a}, i64 0, metadata !14), !dbg !15 ; [debug line = 99:28] [debug variable = a]
  call void (...)* @_ssdm_op_SpecInterface(i32* %a, i8* getelementptr inbounds ([6 x i8]* @.str, i64 0, i64 0), i32 0, i32 0, i32 0, i32 50, i8* getelementptr inbounds ([1 x i8]* @.str1, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8]* @.str1, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8]* @.str1, i64 0, i64 0)) nounwind, !dbg !16 ; [debug line = 101:1]
  call void @llvm.dbg.declare(metadata !{[50 x i32]* %buff}, metadata !18), !dbg !22 ; [debug line = 104:7] [debug variable = buff]
  br label %burst.rd.header

burst.rd.header:                                  ; preds = %burst.rd.body, %0
  %indvar = phi i32 [ 0, %0 ], [ %indvar.next, %burst.rd.body ] ; [#uses=3 type=i32]
  %exitcond1 = icmp eq i32 %indvar, 50            ; [#uses=1 type=i1]
  br i1 %exitcond1, label %burst.rd.end.preheader, label %burst.rd.body

burst.rd.end.preheader:                           ; preds = %burst.rd.header
  br label %burst.rd.end, !dbg !23                ; [debug line = 111:7]

burst.rd.body:                                    ; preds = %burst.rd.header
  %burstread.rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @.str4) nounwind ; [#uses=1 type=i32]
  %1 = call i32 (...)* @_ssdm_op_SpecBurst(i32* %a, i32 1, i32 50) nounwind ; [#uses=0 type=i32]
  %2 = call i32 (...)* @_ssdm_SpecLoopFlatten(i32 1, [1 x i8]* @str3) nounwind ; [#uses=0 type=i32]
  %3 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str2) nounwind ; [#uses=0 type=i32]
  %4 = call i32 (...)* @_ssdm_op_SpecLoopName([14 x i8]* @str1) nounwind ; [#uses=0 type=i32]
  %indvar.next = add i32 %indvar, 1               ; [#uses=1 type=i32]
  %tmp = sext i32 %indvar to i64, !dbg !25        ; [#uses=2 type=i64] [debug line = 109:3]
  %a.addr = getelementptr i32* %a, i64 %tmp, !dbg !25 ; [#uses=1 type=i32*] [debug line = 109:3]
  %a.load = load i32* %a.addr, align 4, !dbg !25  ; [#uses=1 type=i32] [debug line = 109:3]
  %buff.addr = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp, !dbg !25 ; [#uses=1 type=i32*] [debug line = 109:3]
  store i32 %a.load, i32* %buff.addr, align 4, !dbg !25 ; [debug line = 109:3]
  %burstread.rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @.str4, i32 %burstread.rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.rd.header

burst.rd.end:                                     ; preds = %5, %burst.rd.end.preheader
  %i = phi i32 [ %i.1, %5 ], [ 0, %burst.rd.end.preheader ] ; [#uses=3 type=i32]
  %exitcond = icmp eq i32 %i, 50, !dbg !23        ; [#uses=1 type=i1] [debug line = 111:7]
  br i1 %exitcond, label %burst.wr.header.preheader, label %5, !dbg !23 ; [debug line = 111:7]

burst.wr.header.preheader:                        ; preds = %burst.rd.end
  br label %burst.wr.header

; <label>:5                                       ; preds = %burst.rd.end
  %tmp.1 = sext i32 %i to i64, !dbg !26           ; [#uses=1 type=i64] [debug line = 112:5]
  %buff.addr.1 = getelementptr inbounds [50 x i32]* %buff, i64 0, i64 %tmp.1, !dbg !26 ; [#uses=2 type=i32*] [debug line = 112:5]
  %buff.load = load i32* %buff.addr.1, align 4, !dbg !26 ; [#uses=2 type=i32] [debug line = 112:5]
  call void (...)* @_ssdm_SpecKeepArrayLoad(i32 %buff.load) nounwind
  %tmp.2 = add nsw i32 %buff.load, 100, !dbg !26  ; [#uses=1 type=i32] [debug line = 112:5]
  store i32 %tmp.2, i32* %buff.addr.1, align 4, !dbg !26 ; [debug line = 112:5]
  %i.1 = add nsw i32 %i, 1, !dbg !28              ; [#uses=1 type=i32] [debug line = 111:20]
  call void @llvm.dbg.value(metadata !{i32 %i.1}, i64 0, metadata !29), !dbg !28 ; [debug line = 111:20] [debug variable = i]
  br label %burst.rd.end, !dbg !28                ; [debug line = 111:20]

burst.wr.header:                                  ; preds = %burst.wr.body, %burst.wr.header.preheader
  %indvar5 = phi i32 [ %indvar.next6, %burst.wr.body ], [ 0, %burst.wr.header.preheader ] ; [#uses=3 type=i32]
  %exitcond7 = icmp eq i32 %indvar5, 50           ; [#uses=1 type=i1]
  br i1 %exitcond7, label %burst.wr.end, label %burst.wr.body

burst.wr.body:                                    ; preds = %burst.wr.header
  %burstwrite.rbegin = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @.str8) nounwind ; [#uses=1 type=i32]
  %6 = call i32 (...)* @_ssdm_op_SpecBurst(i32* %a, i32 0, i32 50) nounwind ; [#uses=0 type=i32]
  %7 = call i32 (...)* @_ssdm_SpecLoopFlatten(i32 1, [1 x i8]* @str7) nounwind ; [#uses=0 type=i32]
  %8 = call i32 (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @str6) nounwind ; [#uses=0 type=i32]
  %9 = call i32 (...)* @_ssdm_op_SpecLoopName([18 x i8]* @str5) nounwind ; [#uses=0 type=i32]
  %indvar.next6 = add i32 %indvar5, 1             ; [#uses=1 type=i32]
  %tmp.4 = sext i32 %indvar5 to i64, !dbg !30     ; [#uses=2 type=i64] [debug line = 115:3]
  %buff.addr.2 = getelementptr [50 x i32]* %buff, i64 0, i64 %tmp.4, !dbg !30 ; [#uses=1 type=i32*] [debug line = 115:3]
  %buff.load.1 = load i32* %buff.addr.2, align 4, !dbg !30 ; [#uses=1 type=i32] [debug line = 115:3]
  %a.addr.1 = getelementptr i32* %a, i64 %tmp.4, !dbg !30 ; [#uses=1 type=i32*] [debug line = 115:3]
  store i32 %buff.load.1, i32* %a.addr.1, align 4, !dbg !30 ; [debug line = 115:3]
  %burstwrite.rend = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @.str8, i32 %burstwrite.rbegin) nounwind ; [#uses=0 type=i32]
  br label %burst.wr.header

burst.wr.end:                                     ; preds = %burst.wr.header
  ret void, !dbg !31                              ; [debug line = 116:1]
}

; [#uses=1]
declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

; [#uses=1]
declare void @_ssdm_op_SpecInterface(...) nounwind

; [#uses=2]
declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

; [#uses=1]
declare void @_ssdm_SpecKeepArrayLoad(...)

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

!llvm.dbg.cu = !{!0}

!0 = metadata !{i32 786449, i32 0, i32 4, metadata !"C:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master/proj_axi_master/solution1/.autopilot/db/example.pragma.2.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master", metadata !"clang version 3.1 ", i1 true, i1 false, metadata !"", i32 0, metadata !1, metadata !1, metadata !3, metadata !1} ; [ DW_TAG_compile_unit ]
!1 = metadata !{metadata !2}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4}
!4 = metadata !{metadata !5}
!5 = metadata !{i32 786478, i32 0, metadata !6, metadata !"example", metadata !"example", metadata !"_Z7examplePVi", metadata !6, i32 99, metadata !7, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i32*)* @example, null, null, metadata !12, i32 99} ; [ DW_TAG_subprogram ]
!6 = metadata !{i32 786473, metadata !"example.cpp", metadata !"c:/Projects/2014-03-04_TLS-Aspect/FPGA/HLS/Tests/axi_master", null} ; [ DW_TAG_file_type ]
!7 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !8, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!8 = metadata !{null, metadata !9}
!9 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !10} ; [ DW_TAG_pointer_type ]
!10 = metadata !{i32 786485, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !11} ; [ DW_TAG_volatile_type ]
!11 = metadata !{i32 786468, null, metadata !"int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!12 = metadata !{metadata !13}
!13 = metadata !{i32 786468}                      ; [ DW_TAG_base_type ]
!14 = metadata !{i32 786689, metadata !5, metadata !"a", metadata !6, i32 16777315, metadata !9, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!15 = metadata !{i32 99, i32 28, metadata !5, null}
!16 = metadata !{i32 101, i32 1, metadata !17, null}
!17 = metadata !{i32 786443, metadata !5, i32 99, i32 30, metadata !6, i32 0} ; [ DW_TAG_lexical_block ]
!18 = metadata !{i32 786688, metadata !17, metadata !"buff", metadata !6, i32 104, metadata !19, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!19 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 1600, i64 32, i32 0, i32 0, metadata !11, metadata !20, i32 0, i32 0} ; [ DW_TAG_array_type ]
!20 = metadata !{metadata !21}
!21 = metadata !{i32 786465, i64 0, i64 49}       ; [ DW_TAG_subrange_type ]
!22 = metadata !{i32 104, i32 7, metadata !17, null}
!23 = metadata !{i32 111, i32 7, metadata !24, null}
!24 = metadata !{i32 786443, metadata !17, i32 111, i32 3, metadata !6, i32 1} ; [ DW_TAG_lexical_block ]
!25 = metadata !{i32 109, i32 3, metadata !17, null}
!26 = metadata !{i32 112, i32 5, metadata !27, null}
!27 = metadata !{i32 786443, metadata !24, i32 111, i32 24, metadata !6, i32 2} ; [ DW_TAG_lexical_block ]
!28 = metadata !{i32 111, i32 20, metadata !24, null}
!29 = metadata !{i32 786688, metadata !17, metadata !"i", metadata !6, i32 103, metadata !11, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!30 = metadata !{i32 115, i32 3, metadata !17, null}
!31 = metadata !{i32 116, i32 1, metadata !17, null}
