.text
.hsa_code_object_version 2,1
.hsa_code_object_isa 9,0,0,"AMD","AMDGPU"
.weak	_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_ ; -- Begin function _ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_
.p2align	8
.type	_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_,@function
.amdgpu_hsa_kernel _ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_
_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_: ; @_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_
.Lfunc_begin0:
.amd_kernel_code_t
  amd_code_version_major = 1
  amd_code_version_minor = 1
  amd_machine_kind = 1
  amd_machine_version_major = 9
  amd_machine_version_minor = 0
  amd_machine_version_stepping = 0
  kernel_code_entry_byte_offset = 256
  kernel_code_prefetch_byte_size = 0
  max_scratch_backing_memory_byte_size = 0
  granulated_workitem_vgpr_count = 2
  granulated_wavefront_sgpr_count = 0
  priority = 0
  float_mode = 192
  priv = 0
  enable_dx10_clamp = 1
  debug_mode = 0
  enable_ieee_mode = 1
  enable_sgpr_private_segment_wave_byte_offset = 0
  user_sgpr_count = 6
  enable_trap_handler = 1
  enable_sgpr_workgroup_id_x = 1
  enable_sgpr_workgroup_id_y = 0
  enable_sgpr_workgroup_id_z = 0
  enable_sgpr_workgroup_info = 0
  enable_vgpr_workitem_id = 0
  enable_exception_msb = 0
  granulated_lds_size = 0
  enable_exception = 0
  enable_sgpr_private_segment_buffer = 1
  enable_sgpr_dispatch_ptr = 0
  enable_sgpr_queue_ptr = 0
  enable_sgpr_kernarg_segment_ptr = 1
  enable_sgpr_dispatch_id = 0
  enable_sgpr_flat_scratch_init = 0
  enable_sgpr_private_segment_size = 0
  enable_sgpr_grid_workgroup_count_x = 0
  enable_sgpr_grid_workgroup_count_y = 0
  enable_sgpr_grid_workgroup_count_z = 0
  enable_ordered_append_gds = 0
  private_element_size = 1
  is_ptr64 = 1
  is_dynamic_callstack = 0
  is_debug_enabled = 0
  is_xnack_enabled = 0
  workitem_private_segment_byte_size = 0
  workgroup_group_segment_byte_size = 0
  gds_segment_byte_size = 0
  kernarg_segment_byte_size = 24
  workgroup_fbarrier_count = 0
  wavefront_sgpr_count = 8
  workitem_vgpr_count = 9
  reserved_vgpr_first = 0
  reserved_vgpr_count = 0
  reserved_sgpr_first = 0
  reserved_sgpr_count = 0
  debug_wavefront_private_segment_offset_sgpr = 0
  debug_private_segment_buffer_sgpr = 0
  kernarg_segment_alignment = 4
  group_segment_alignment = 4
  private_segment_alignment = 4
  wavefront_size = 6
  call_convention = -1
  runtime_loader_kernel_symbol = 0
.end_amd_kernel_code_t
; BB#0:
s_load_dwordx2 s[2:3], s[4:5], 0x8
s_load_dwordx2 s[0:1], s[4:5], 0x0
v_lshlrev_b32_e32 v4, 2, v0
s_load_dwordx2 s[4:5], s[4:5], 0x10
v_mov_b32_e32 v5, 0
s_waitcnt lgkmcnt(0)
v_add_i32_e32 v2, vcc, s2, v4
v_mov_b32_e32 v3, s3
v_addc_u32_e32 v3, vcc, v3, v5, vcc
v_mov_b32_e32 v0, s0
v_add_i32_e32 v4, vcc, s4, v4
v_mov_b32_e32 v6, s5
v_mov_b32_e32 v1, s1
v_addc_u32_e32 v5, vcc, v6, v5, vcc
flat_load_dword v2, v[2:3] ; inImage
flat_load_dword v3, v[0:1] ; wImage[0]
flat_load_dword v8, v[4:5] ; outImage
flat_load_dword v6, v[0:1] offset:4 ; wImage[1]
s_waitcnt vmcnt(0) lgkmcnt(0)
v_mac_f32_e32 v8, v2, v3  ; v8 = in * wImage[0] + v8
v_mul_f32 v7, v2, v6      ; v7 = in * wImage[1]
v_add_f32 v8, v7, v8 row_shl:1 bound_ctrl:0        ; v8 = v8 + v7
flat_store_dword v[4:5], v8
s_endpgm
.Lfunc_end0:
.size	_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_, .Lfunc_end0-_ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_
                                      ; -- End function
.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 160
; NumSgprs: 8
; NumVgprs: 9
; ScratchSize: 0
; FloatMode: 192
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 0
; VGPRBlocks: 2
; NumSGPRsForWavesPerEU: 8
; NumVGPRsForWavesPerEU: 9
; ReservedVGPRFirst: 0
; ReservedVGPRCount: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 1
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0

.ident	"HCC clang version 6.0.0  (based on HCC 1.0.17333-abbf79f-c0495b7-2333eab )"
.section	".note.GNU-stack"
.amdgpu_code_object_metadata
---
Version:         [ 1, 0 ]
Kernels:
- Name:            _ZZ4mainEN67HIP_kernel_functor_name_begin_unnamed_HIP_kernel_functor_name_end_319__cxxamp_trampolineEPfS0_S0_
  Language:        OpenCL C
  LanguageVersion: [ 2, 0 ]
  Args:
    - Size:            8
      Align:           8
      ValueKind:       GlobalBuffer
      ValueType:       F32
      AddrSpaceQual:   Generic
    - Size:            8
      Align:           8
      ValueKind:       GlobalBuffer
      ValueType:       F32
      AddrSpaceQual:   Generic
    - Size:            8
      Align:           8
      ValueKind:       GlobalBuffer
      ValueType:       F32
      AddrSpaceQual:   Generic
    - Size:            8
      Align:           8
      ValueKind:       HiddenGlobalOffsetX
      ValueType:       I64
    - Size:            8
      Align:           8
      ValueKind:       HiddenGlobalOffsetY
      ValueType:       I64
    - Size:            8
      Align:           8
      ValueKind:       HiddenGlobalOffsetZ
      ValueType:       I64
  CodeProps:
    KernargSegmentSize: 24
    WavefrontNumSGPRs: 8
    WorkitemNumVGPRs: 9
    KernargSegmentAlign: 4
    GroupSegmentAlign: 4
    PrivateSegmentAlign: 4
    WavefrontSize:   6
...
.end_amdgpu_code_object_metadata
