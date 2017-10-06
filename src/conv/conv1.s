	.text
	.hsa_code_object_version 2,1
	.hsa_code_object_isa 9,0,0,"AMD","AMDGPU"
	.globl	hello_world             ; -- Begin function hello_world
	.p2align	8
	.type	hello_world,@function
	.amdgpu_hsa_kernel hello_world
hello_world:                            ; @hello_world
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
		granulated_workitem_vgpr_count = 1
		granulated_wavefront_sgpr_count = 1
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
		wavefront_sgpr_count = 10
		workitem_vgpr_count = 24
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
	s_load_dwordx2 s[6:7], s[4:5], 0x8
	s_load_dwordx2 s[0:1], s[4:5], 0x0
	v_lshlrev_b32_e32 v2, 2, v0
	s_load_dwordx2 s[4:5], s[4:5], 0x10
	v_mov_b32_e32 v3, 0
	s_waitcnt lgkmcnt(0)
	v_add_i32_e32 v0, vcc, s6, v2
	v_mov_b32_e32 v1, s7
	v_addc_u32_e32 v1, vcc, v1, v3, vcc
	v_add_i32_e32 v2, vcc, s4, v2
	v_mov_b32_e32 v4, s5
	v_addc_u32_e32 v3, vcc, v4, v3, vcc
	global_load_dword v5, v[0:1], off
        global_load_dword v6, v[0:1], off offset:64
        global_load_dword v7, v[0:1], off offset:128
;        global_load_dword v8, v[0:1], off offset:192

;        global_load_dword v9, v[0:1], off offset:256
;        global_load_dword v10, v[0:1], off offset:320
;        global_load_dword v11, v[0:1], off offset:384
;        global_load_dword v12, v[0:1], off offset:448

;        global_load_dword v13, v[0:1], off offset:512
;        global_load_dword v14, v[0:1], off offset:576
;        global_load_dword v15, v[0:1], off offset:640
;        global_load_dword v16, v[0:1], off offset:704

;        global_load_dword v17, v[0:1], off offset:768
;        global_load_dword v18, v[0:1], off offset:832
;        global_load_dword v19, v[0:1], off offset:896
;        global_load_dword v20, v[0:1], off offset:960

	global_load_dword v4, v[2:3], off
	s_load_dwordx4 s[4:7], s[0:1], 0x0
        s_load_dwordx4 s[8:11], s[0:1], 0x10
        s_load_dwordx4 s[12:15], s[0:1], 0x20
        s_load_dword s16, s[0:1], 0x30
        s_waitcnt vmcnt(0) lgkmcnt(0)
;
        v_mac_f32 v4, s4, v5
        v_mul_f32 v1, s5, v5
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:1 bound_ctrl:0
        v_mul_f32 v1, s6, v5
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:2 bound_ctrl:0
        v_mul_f32 v1, s7, v5
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:3 bound_ctrl:0
;
        v_mac_f32 v4, s8, v6
        v_mul_f32 v1, s9, v6
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:1 bound_ctrl:0
        v_mul_f32 v1, s10, v6
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:2 bound_ctrl:0
        v_mul_f32 v1, s11, v6
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:3 bound_ctrl:0
;
        v_mac_f32 v4, s12, v7
        v_mul_f32 v1, s13, v7
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:1 bound_ctrl:0
        v_mul_f32 v1, s14, v7
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:2 bound_ctrl:0
        v_mul_f32 v1, s15, v7
        v_nop
        v_nop
        v_add_f32 v4, v1, v4 row_shl:3 bound_ctrl:0 
;
	global_store_dword v[2:3], v4, off
	s_endpgm
.Lfunc_end0:
	.size	hello_world, .Lfunc_end0-hello_world
                                        ; -- End function
	.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 144
; NumSgprs: 10
; NumVgprs: 1
; ScratchSize: 0
; FloatMode: 192
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 1
; VGPRBlocks: 1
; NumSGPRsForWavesPerEU: 10
; NumVGPRsForWavesPerEU: 6
; ReservedVGPRFirst: 0
; ReservedVGPRCount: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 1
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0

	.section	".note.GNU-stack"
	.amdgpu_code_object_metadata
---
Version:         [ 1, 0 ]
Kernels:         
  - Name:            hello_world
    Args:            
      - Size:            8
        Align:           8
        ValueKind:       GlobalBuffer
        ValueType:       F32
        AddrSpaceQual:   Global
      - Size:            8
        Align:           8
        ValueKind:       GlobalBuffer
        ValueType:       F32
        AddrSpaceQual:   Global
      - Size:            8
        Align:           8
        ValueKind:       GlobalBuffer
        ValueType:       F32
        AddrSpaceQual:   Global
    CodeProps:       
      KernargSegmentSize: 24
      WavefrontNumSGPRs: 10
      WorkitemNumVGPRs: 1
      KernargSegmentAlign: 4
      GroupSegmentAlign: 4
      PrivateSegmentAlign: 4
      WavefrontSize:   6
...
	.end_amdgpu_code_object_metadata
