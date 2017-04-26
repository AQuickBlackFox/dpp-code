declare i32 @llvm.amdgcn.workitem.id.x()

define linkonce_odr float @qp1(float) {
  %ret = tail call float asm "v_mov_b32 $0, $1 quad_perm:[2,2,2,2]", "=v,v"(float %0)
  ret float %ret
}

define amdgpu_kernel void @Run(float addrspace(1)* %in, float addrspace(1)* %out){
  %tid = call i32 @llvm.amdgcn.workitem.id.x()
  %1 = getelementptr float, float addrspace(1)* %in, i32 %tid
  %2 = getelementptr float, float addrspace(1)* %out, i32 %tid
  %3 = load float, float addrspace(1)* %1
  %4 = call float @qp1(float %3)
  store float %4, float addrspace(1)* %2
  ret void
}
