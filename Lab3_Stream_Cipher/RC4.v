`timescale 1ns / 1ps


module RC4(
    input [127:0] data,
    input [127:0] init_key,
    output [127:0] key,
    output [127:0] enc_data
);


reg clk;
reg [7:0] plaintext[15:0];
reg [7:0] ciphertext[15:0];
reg [7:0] key_gen[15:0];
reg [7:0] seedkey[15:0];
reg [7:0] S[255:0];
reg [7:0] T[255:0];
reg [8:0] i,j,k,t,temp;


initial begin
    clk = 0;
    for(i=0;i<16;i=i+1)
    begin
        plaintext[i]=8'h01;                  // 初始化16字节明文为0x01
        ciphertext[i]=8'h0;
        key_gen[i]=8'h0;
        seedkey[i]=i;                       // 初始化密钥种子为seed[i]=i
    end
    forever 
        #5 clk=~clk;                        // 自定义仿真时钟
end


always@(posedge clk)
    begin
            for(i=0;i<256;i=i+1)            // 初始化S盒、T盒
            begin
                S[i]=i;
                T[i]=seedkey[i%16];         // 使用初始种子满T表
            end
        
            j=0;
            for(i=0;i<256;i=i+1)            // 根据T盒调整S盒
            begin
                j=(j+S[i]+T[i])%256;
                temp=S[i];                  // swap(S[i],S[j])
                S[i]=S[j];
                S[j]=temp;
            end
        
            i=0;j=0;
            for(k=0;i<16;k=k+1)             // 导出密钥并加密
            begin
                i=(i+1)%256;
                j=(j+S[i])%256;
                temp=S[i];                  // swap(S[i],S[j])
                S[i]=S[j];
                S[j]=temp;
                t=(S[i]+S[j])%256;
                key_gen[k]=S[t];            // 导出密钥
                ciphertext[k]=key_gen[k]^plaintext[k];   // 加密明文
            end
    end
endmodule

