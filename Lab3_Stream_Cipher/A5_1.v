`timescale 1ns / 1ps


module A5_1(
    input [63:0] data,
    input [63:0] init_key,
    output [63:0] key,
    output [63:0] enc_data
);

reg clk;
reg [63:0] plaintext;
reg [63:0] ciphertext;
reg [63:0] key_gen;
reg [63:0] temp_init;
reg [18:0] X;
reg [21:0] Y;
reg [22:0] Z;
reg [5:0]counter;
reg M,key_bit;


//initial begin
//    clk = 1'b0;
//    plaintext = data;
//    X = init_key[18:0];
//    Y = init_key[40:19];
//    Z = init_key[63:41];
//end


initial begin
    clk = 0;
    counter = 6'b0;
    plaintext = 64'hffffffffffffffff;
    ciphertext = 64'h0;
    key_gen = 64'h0;
    
    temp_init = 64'hffffffffffffffff;    // 初始化三个移位寄存器
    X = temp_init[18:0];
    Y = temp_init[40:19];
    Z = temp_init[63:41];
    forever 
        #5 clk=~clk;                      // 自定义仿真时钟
end


always@(posedge clk)
    begin
    if(counter<=6'b111111)              // 输出64位密钥并加密明文
        begin
    	    M = X[8]&Y[10] | Y[10]&Z[10] | X[8]&Z[10];		       // 真值表等价于X[8]、Y[10]和Z[10]的"投票结果"
    	   
            if(M==X[8])                                            // 等于投票结果则进行移位
                begin
                    X = { X[17:0] , X[13]^X[16]^X[17]^X[18] };     // 寄存器左移1位并填充低位
                end
            
            if(M==Y[10])                                            // 等于投票结果则进行移位
                begin
                    Y = { Y[20:0] , Y[20]^Y[21] };                  // 寄存器左移1位并填充低位
                end
            
            if(M==Z[10])                                            // 等于投票结果则进行移位
                begin
                    Z = { Z[21:0] , Z[7]^Z[20]^Z[21]^Z[22] };       // 寄存器左移1位并填充低位
                end
                
            key_bit = X[18]^Y[21]^Z[22];                            // 输出密钥
            key_gen[counter] = key_bit;
            ciphertext[counter] = key_bit^plaintext[counter];       // 加密明文
            counter = counter + 1;
        end
    end
    assign enc_data = ciphertext;                                   // 加密结果连续赋值给输出
    assign key = key_gen;                                           // 导出密钥连续赋值给输出
endmodule
