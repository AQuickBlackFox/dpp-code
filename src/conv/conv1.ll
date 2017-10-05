define amdgpu_kernel void @hello_world(float addrspace(1)* noalias, float addrspace(1)* noalias, float addrspace(1)* noalias) {
    %tid = call i32 @llvm.amdgcn.workitem.id.x() readnone
    %tid1 = add i32 %tid, 16
    %tid2 = add i32 %tid, 32
    %tid3 = add i32 %tid, 48
    %wImagePtr0 = getelementptr float, float addrspace(1)* %0, i32 0
    %wImagePtr1 = getelementptr float, float addrspace(1)* %0, i32 1
    %wImagePtr2 = getelementptr float, float addrspace(1)* %0, i32 2
    %wImagePtr3 = getelementptr float, float addrspace(1)* %0, i32 3
    %inImagePtr0 = getelementptr float, float addrspace(1)* %1, i32 %tid
    %inImagePtr1 = getelementptr float, float addrspace(1)* %1, i32 %tid1
    %inImagePtr2 = getelementptr float, float addrspace(1)* %1, i32 %tid2
    %inImagePtr3 = getelementptr float, float addrspace(1)* %1, i32 %tid3
    %outImagePtr = getelementptr float, float addrspace(1)* %2, i32 %tid
    %wImage0 = load float, float addrspace(1)* %wImagePtr0
    %wImage1 = load float, float addrspace(1)* %wImagePtr1
    %wImage2 = load float, float addrspace(1)* %wImagePtr2
    %wImage3 = load float, float addrspace(1)* %wImagePtr3
    %inImage0 = load float, float addrspace(1)* %inImagePtr1
    %outImage = load float, float addrspace(1)* %outImagePtr
    %4 = fmul float %wImage0, %inImage
    %5 = fadd float %4, %outImage
    %6 = fmul float %wImage1, %inImage
    %7 = fmul float %wImage2, %inImage
    %8 = fmul float %wImage3, %inImage
    %9 = tail call float asm "v_add_f32 $0, $1, $2 row_shl:1 bound_ctrl:0", "=v,v,v"(float %6, float %5)
    %10 = tail call float asm "v_add_f32 $0, $1, $2 row_shl:2 bound_ctrl:0", "=v,v,v"(float %7, float %9)
    %11 = tail call float asm "v_add_f32 $0, $1, $2 row_shl:3 bound_ctrl:0", "=v,v,v"(float %8, float %10)
    store float %11, float addrspace(1)* %outImagePtr
    ret void
}

declare i32 @llvm.amdgcn.workitem.id.x()
