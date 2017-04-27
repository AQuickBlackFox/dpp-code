declare i32 @llvm.amdgcn.workitem.id.x()

define linkonce_odr float @tp_0(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[0,0,0,0]","=v,v"(float %0)
  ret float %ret
}

define linkonce_odr void @tp_1(float, float, float) {
;  %4 = tail call float asm "v_mov_b32 $0, $1 quad_perm:[1,1,1,1]","=v,v"(float %2)
;  tail call void asm sideeffect "v_mac_f32 $0, $1, $2","v,v,v"(float %0, float %1, float %4)
  tail call void asm sideeffect "v_mac_f32 $0, $1, $2 quad_perm:[1,1,1,1]","v,v,v"(float %0, float %1, float %2)
  ret void
}

define linkonce_odr void @tp_2(float, float, float) {
;  %4 = tail call float asm "v_mov_b32 $0, $1 quad_perm:[2,2,2,2]","=v,v"(float %2)
;  tail call void asm sideeffect "v_mac_f32 $0, $1, $2","v,v,v"(float %0, float %1, float %4)
  tail call void asm sideeffect "v_mac_f32 $0, $1, $2 quad_perm:[2,2,2,2]","v,v,v"(float %0, float %1, float %2)
  ret void
}

define linkonce_odr void @tp_3(float, float, float) {
;  %4 = tail call float asm "v_mov_b32 $0, $1 quad_perm:[3,3,3,3]","=v,v"(float %2)
;  tail call void asm sideeffect "v_mac_f32 $0, $1, $2","v,v,v"(float %0, float %1, float %4)
  tail call void asm sideeffect "v_mac_f32 $0, $1, $2 quad_perm:[3,3,3,3]","v,v,v"(float %0, float %1, float %2)
  ret void
}


define amdgpu_kernel void @Run(float addrspace(1)* %vertex, float addrspace(1)* %transform, float addrspace(1)* %output)
{
  %tid0 = call i32 @llvm.amdgcn.workitem.id.x()
  %tid1 = add i32 4,  %tid0
  %tid2 = add i32 8,  %tid0
  %tid3 = add i32 12, %tid0

  %vgep = getelementptr float, float addrspace(1)* %vertex, i32 %tid0
  %ogep = getelementptr float, float addrspace(1)* %output, i32 %tid0

  %tgep0 = getelementptr float, float addrspace(1)* %transform, i32 %tid0
  %tgep1 = getelementptr float, float addrspace(1)* %transform, i32 %tid1
  %tgep2 = getelementptr float, float addrspace(1)* %transform, i32 %tid2
  %tgep3 = getelementptr float, float addrspace(1)* %transform, i32 %tid3

  %t0 = load float, float addrspace(1)* %tgep0
  %t1 = load float, float addrspace(1)* %tgep1
  %t2 = load float, float addrspace(1)* %tgep2
  %t3 = load float, float addrspace(1)* %tgep3

  %v = load float, float addrspace(1)* %vgep

  %tmp0 = tail call float @tp_0(float %v)
  %o0 = fmul float %t0, %tmp0

  tail call void @tp_1(float %o0, float %v, float %t1)

  tail call void @tp_2(float %o0, float %v, float %t2)
 
  tail call void @tp_3(float %o0, float %v, float %t3)

  store float %o0, float addrspace(1)* %ogep
  ret void
}

