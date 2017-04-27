declare i32 @llvm.amdgcn.workitem.id.x()

define linkonce_odr float @tp_0(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[0,0,0,0]","=v,v"(float %0)
  ret float %ret
}

define linkonce_odr float @tp_1(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[1,1,1,1]","=v,v"(float %0)
  ret float %ret
}

define linkonce_odr float @tp_2(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[2,2,2,2]","=v,v"(float %0)
  ret float %ret
}

define linkonce_odr float @tp_3(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[3,3,3,3]","=v,v"(float %0)
  ret float %ret
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

  %tmp1 = tail call float @tp_1(float %v)
  %o1 = fmul float %t1, %tmp1
  %o11 = fadd float %o1, %o0

  %tmp2 = tail call float @tp_2(float %v)
  %o2 = fmul float %t2, %tmp2
  %o22 = fadd float %o2, %o11

  %tmp3 = tail call float @tp_3(float %v)
  %o3 = fmul float %t3, %tmp3
  %o33 = fadd float %o3, %o22

  store float %o33, float addrspace(1)* %ogep
  ret void
}

