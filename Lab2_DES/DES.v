`timescale 1ns / 1ps

module KeyGen(
input [64:1] key,
output reg [48:1] key1, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12, key13, key14, key15, key16
);

function [56:1] shuffle_PC1(input [64:1] key_in);
    integer PC1_Table[56:1],i;
    reg [56:1] key_out;
    
    begin
    PC1_Table[1] = 57;PC1_Table[2] = 49;PC1_Table[3] = 41;PC1_Table[4] = 33;PC1_Table[5] = 25;PC1_Table[6] = 17;PC1_Table[7] = 9;PC1_Table[8] = 1;PC1_Table[9] = 58;PC1_Table[10] = 50;PC1_Table[11] = 42;PC1_Table[12] = 34;PC1_Table[13] = 26;PC1_Table[14] = 18;
    PC1_Table[15] = 10;PC1_Table[16] = 2;PC1_Table[17] = 59;PC1_Table[18] = 51;PC1_Table[19] = 43;PC1_Table[20] = 35;PC1_Table[21] = 27;PC1_Table[22] = 19;PC1_Table[23] = 11;PC1_Table[24] = 3;PC1_Table[25] = 60;PC1_Table[26] = 52;PC1_Table[27] = 44;PC1_Table[28] = 36;
    PC1_Table[29] = 63;PC1_Table[30] = 55;PC1_Table[31] = 47;PC1_Table[32] = 39;PC1_Table[33] = 31;PC1_Table[34] = 23;PC1_Table[35] = 15;PC1_Table[36] = 7;PC1_Table[37] = 62;PC1_Table[38] = 54;PC1_Table[39] = 46;PC1_Table[40] = 38;PC1_Table[41] = 30;PC1_Table[42] = 22;
    PC1_Table[43] = 14;PC1_Table[44] = 6;PC1_Table[45] = 61;PC1_Table[46] = 53;PC1_Table[47] = 45;PC1_Table[48] = 37;PC1_Table[49] = 29;PC1_Table[50] = 21;PC1_Table[51] = 13;PC1_Table[52] = 5;PC1_Table[53] = 28;PC1_Table[54] = 20;PC1_Table[55] = 12;PC1_Table[56] = 4;

    for(i=1; i<=56; i=i+1)
        key_out[56-i+1] = key_in[64-PC1_Table[i]+1];
    shuffle_PC1 = key_out;
    
    end
endfunction


function [48:1] shuffle_PC2(input [56:1] key_in);
    integer PC2_Table[48:1],i;
    reg [48:1] key_out;
    
    begin 
    PC2_Table[1] = 14;PC2_Table[2] = 17;PC2_Table[3] = 11;PC2_Table[4] = 24;PC2_Table[5] = 1;PC2_Table[6] = 5;PC2_Table[7] = 3;PC2_Table[8] = 28;PC2_Table[9] = 15;PC2_Table[10] = 6;PC2_Table[11] = 21;PC2_Table[12] = 10;
    PC2_Table[13] = 23;PC2_Table[14] = 19;PC2_Table[15] = 12;PC2_Table[16] = 4;PC2_Table[17] = 26;PC2_Table[18] = 8;PC2_Table[19] = 16;PC2_Table[20] = 7;PC2_Table[21] = 27;PC2_Table[22] = 20;PC2_Table[23] = 13;PC2_Table[24] = 2;
    PC2_Table[25] = 41;PC2_Table[26] = 52;PC2_Table[27] = 31;PC2_Table[28] = 37;PC2_Table[29] = 47;PC2_Table[30] = 55;PC2_Table[31] = 30;PC2_Table[32] = 40;PC2_Table[33] = 51;PC2_Table[34] = 45;PC2_Table[35] = 33;PC2_Table[36] = 48;
    PC2_Table[37] = 44;PC2_Table[38] = 49;PC2_Table[39] = 39;PC2_Table[40] = 56;PC2_Table[41] = 34;PC2_Table[42] = 53;PC2_Table[43] = 46;PC2_Table[44] = 42;PC2_Table[45] = 50;PC2_Table[46] = 36;PC2_Table[47] = 29;PC2_Table[48] = 32;

	for(i=1; i<=48; i=i+1)
        key_out[48-i+1] = key_in[56-PC2_Table[i]+1];
    shuffle_PC2 = key_out;
    
    end
endfunction


function [56:1] Shift_left(input [28:1] left,input [28:1] right,input integer round_id );
    integer Step_Table[1:16];
    
    begin
    Step_Table[1] = 1;Step_Table[2] = 1;Step_Table[3] = 2;Step_Table[4] = 2;Step_Table[5] = 2;Step_Table[6] = 2;Step_Table[7] = 2;Step_Table[8] = 2;
    Step_Table[9] = 1;Step_Table[10] = 2;Step_Table[11] = 2;Step_Table[12] = 2;Step_Table[13] = 2;Step_Table[14] = 2;Step_Table[15] = 2;Step_Table[16] = 1;			
      
    if(Step_Table[round_id] == 'd1)
       Shift_left = {left[27:1], left[28], right[27:1], right[28]};
    else
       Shift_left = {left[26:1], left[28:27], right[26:1], right[28:27]};
      
    end
endfunction

    reg [56:1] PC1_out;
    reg [28:1] Left, Right;
    reg [28:1] Temp_Left, Temp_Right;
    reg [48:1] Key[1:16];
    integer i;

always @(key)
    begin
    PC1_out = shuffle_PC1(key);
    Left = PC1_out[56:29];
    Right = PC1_out[28:1];
    
    for(i=1; i<=16; i=i+1)
        begin
        {Temp_Left, Temp_Right} = Shift_left(Left, Right,i);
        Key[i] = shuffle_PC2({Temp_Left, Temp_Right});
        Left = Temp_Left; Right = Temp_Right;
        end
    
    key1 = Key[1];key2 = Key[2];key3 = Key[3];key4 = Key[4];key5 = Key[5];key6 = Key[6];key7 = Key[7];key8 = Key[8];
    key9 = Key[9];key10 = Key[10];key11 = Key[11];key12 = Key[12];key13 = Key[13];key14 = Key[14];key15 = Key[15];key16 = Key[16];

    end
endmodule



module DES(
input [64:1] plaintext,
input [64:1] key,
input is_dec,
output reg [64:1] ciphertext
);

function [64:1] shuffle_IP(input [64:1] data_in);
    integer IP_Table[64:1],i;
    reg [64:1] data_out;

    begin
    IP_Table[1] = 58;IP_Table[2] = 50;IP_Table[3] = 42;IP_Table[4] = 34;IP_Table[5] = 26;IP_Table[6] = 18;IP_Table[7] = 10;IP_Table[8] = 2;IP_Table[9] = 60;IP_Table[10] = 52;IP_Table[11] = 44;IP_Table[12] = 36;IP_Table[13] = 28;IP_Table[14] = 20;IP_Table[15] = 12;IP_Table[16] = 4;
    IP_Table[17] = 62;IP_Table[18] = 54;IP_Table[19] = 46;IP_Table[20] = 38;IP_Table[21] = 30;IP_Table[22] = 22;IP_Table[23] = 14;IP_Table[24] = 6;IP_Table[25] = 64;IP_Table[26] = 56;IP_Table[27] = 48;IP_Table[28] = 40;IP_Table[29] = 32;IP_Table[30] = 24;IP_Table[31] = 16;IP_Table[32] = 8;
    IP_Table[33] = 57;IP_Table[34] = 49;IP_Table[35] = 41;IP_Table[36] = 33;IP_Table[37] = 25;IP_Table[38] = 17;IP_Table[39] = 9;IP_Table[40] = 1;IP_Table[41] = 59;IP_Table[42] = 51;IP_Table[43] = 43;IP_Table[44] = 35;IP_Table[45] = 27;IP_Table[46] = 19;IP_Table[47] = 11;IP_Table[48] = 3;
    IP_Table[49] = 61;IP_Table[50] = 53;IP_Table[51] = 45;IP_Table[52] = 37;IP_Table[53] = 29;IP_Table[54] = 21;IP_Table[55] = 13;IP_Table[56] = 5;IP_Table[57] = 63;IP_Table[58] = 55;IP_Table[59] = 47;IP_Table[60] = 39;IP_Table[61] = 31;IP_Table[62] = 23;IP_Table[63] = 15;IP_Table[64] = 7;

	for(i=1; i<=64; i=i+1)
        data_out[64-i+1] = data_in[64-IP_Table[i]+1];
     shuffle_IP = data_out;      
    end
endfunction

function [64:1] shuffle_invIP(input [64:1] data_in);
    integer IP_inv_Table[64:1],i;
    reg [64:1] data_out;
    
 	begin
    IP_inv_Table[1] = 40;IP_inv_Table[2] = 8;IP_inv_Table[3] = 48;IP_inv_Table[4] = 16;IP_inv_Table[5] = 56;IP_inv_Table[6] = 24;IP_inv_Table[7] = 64;IP_inv_Table[8] = 32;IP_inv_Table[9] = 39;IP_inv_Table[10] = 7;IP_inv_Table[11] = 47;IP_inv_Table[12] = 15;IP_inv_Table[13] = 55;IP_inv_Table[14] = 23;IP_inv_Table[15] = 63;IP_inv_Table[16] = 31;
    IP_inv_Table[17] = 38;IP_inv_Table[18] = 6;IP_inv_Table[19] = 46;IP_inv_Table[20] = 14;IP_inv_Table[21] = 54;IP_inv_Table[22] = 22;IP_inv_Table[23] = 62;IP_inv_Table[24] = 30;IP_inv_Table[25] = 37;IP_inv_Table[26] = 5;IP_inv_Table[27] = 45;IP_inv_Table[28] = 13;IP_inv_Table[29] = 53;IP_inv_Table[30] = 21;IP_inv_Table[31] = 61;IP_inv_Table[32] = 29;
    IP_inv_Table[33] = 36;IP_inv_Table[34] = 4;IP_inv_Table[35] = 44;IP_inv_Table[36] = 12;IP_inv_Table[37] = 52;IP_inv_Table[38] = 20;IP_inv_Table[39] = 60;IP_inv_Table[40] = 28;IP_inv_Table[41] = 35;IP_inv_Table[42] = 3;IP_inv_Table[43] = 43;IP_inv_Table[44] = 11;IP_inv_Table[45] = 51;IP_inv_Table[46] = 19;IP_inv_Table[47] = 59;IP_inv_Table[48] = 27;
    IP_inv_Table[49] = 34;IP_inv_Table[50] = 2;IP_inv_Table[51] = 42;IP_inv_Table[52] = 10;IP_inv_Table[53] = 50;IP_inv_Table[54] = 18;IP_inv_Table[55] = 58;IP_inv_Table[56] = 26;IP_inv_Table[57] = 33;IP_inv_Table[58] = 1;IP_inv_Table[59] = 41;IP_inv_Table[60] = 9;IP_inv_Table[61] = 49;IP_inv_Table[62] = 17;IP_inv_Table[63] = 57;IP_inv_Table[64] = 25;

	for(i=1; i<=64; i=i+1)
        data_out[64-i+1] = data_in[64-IP_inv_Table[i]+1];
    shuffle_invIP = data_out;
 	end
endfunction


function [48:1] shuffle_E(input [32:1] data_in);
    integer E_Table[48:1],i;
    reg [48:1] data_out;
    
    begin
    E_Table[1] = 32;E_Table[2] = 1;E_Table[3] = 2;E_Table[4] = 3;E_Table[5] = 4;E_Table[6] = 5;E_Table[7] = 4;E_Table[8] = 5;E_Table[9] = 6;E_Table[10] = 7;E_Table[11] = 8;E_Table[12] = 9;
    E_Table[13] = 8;E_Table[14] = 9;E_Table[15] = 10;E_Table[16] = 11;E_Table[17] = 12;E_Table[18] = 13;E_Table[19] = 12;E_Table[20] = 13;E_Table[21] = 14;E_Table[22] = 15;E_Table[23] = 16;E_Table[24] = 17;
    E_Table[25] = 16;E_Table[26] = 17;E_Table[27] = 18;E_Table[28] = 19;E_Table[29] = 20;E_Table[30] = 21;E_Table[31] = 20;E_Table[32] = 21;E_Table[33] = 22;E_Table[34] = 23;E_Table[35] = 24;E_Table[36] = 25;
    E_Table[37] = 24;E_Table[38] = 25;E_Table[39] = 26;E_Table[40] = 27;E_Table[41] = 28;E_Table[42] = 29;E_Table[43] = 28;E_Table[44] = 29;E_Table[45] = 30;E_Table[46] = 31;E_Table[47] = 32;E_Table[48] = 1;

    for(i=1; i<=48; i=i+1)
        data_out[48-i+1] = data_in[32-E_Table[i]+1];
    shuffle_E = data_out;
    end
endfunction


function [32:1] shuffle_P(input [32:1] data_in);
    integer P_Table[32:1],i;
    reg [32:1] data_out;
    
    begin
    P_Table[1] = 16;P_Table[2] = 7;P_Table[3] = 20;P_Table[4] = 21;P_Table[5] = 29;P_Table[6] = 12;P_Table[7] = 28;P_Table[8] = 17;
    P_Table[9] = 1;P_Table[10] = 15;P_Table[11] = 23;P_Table[12] = 26;P_Table[13] = 5;P_Table[14] = 18;P_Table[15] = 31;P_Table[16] = 10;
    P_Table[17] = 2;P_Table[18] = 8;P_Table[19] = 24;P_Table[20] = 14;P_Table[21] = 32;P_Table[22] = 27;P_Table[23] = 3;P_Table[24] = 9;
    P_Table[25] = 19;P_Table[26] = 13;P_Table[27] = 30;P_Table[28] = 6;P_Table[29] = 22;P_Table[30] = 11;P_Table[31] = 4;P_Table[32] = 25;

    for(i=1; i<=32; i=i+1)
        data_out[32-i+1] = data_in[32-P_Table[i]+1];
    shuffle_P = data_out;
    end
endfunction


function [48:1] S_BOX(input [48:1] data_in);
    reg [48:1] data_out;
    reg [3:0] S_Box1[3:0][15:0];
    reg [3:0] S_Box2[3:0][15:0];
    reg [3:0] S_Box3[3:0][15:0];
    reg [3:0] S_Box4[3:0][15:0];
    reg [3:0] S_Box5[3:0][15:0];
    reg [3:0] S_Box6[3:0][15:0];
    reg [3:0] S_Box7[3:0][15:0];
    reg [3:0] S_Box8[3:0][15:0];
    
    begin
    S_Box1[0][0] = 14;S_Box1[0][1] = 4;S_Box1[0][2] = 13;S_Box1[0][3] = 1;S_Box1[0][4] = 2;S_Box1[0][5] = 15;S_Box1[0][6] = 11;S_Box1[0][7] = 8;S_Box1[0][8] = 3;S_Box1[0][9] = 10;S_Box1[0][10] = 6;S_Box1[0][11] = 12;S_Box1[0][12] = 5;S_Box1[0][13] = 9;S_Box1[0][14] = 0;S_Box1[0][15] = 7;S_Box1[1][0] = 0;S_Box1[1][1] = 15;S_Box1[1][2] = 7;S_Box1[1][3] = 4;S_Box1[1][4] = 14;S_Box1[1][5] = 2;S_Box1[1][6] = 13;S_Box1[1][7] = 1;S_Box1[1][8] = 10;S_Box1[1][9] = 6;S_Box1[1][10] = 12;S_Box1[1][11] = 11;S_Box1[1][12] = 9;S_Box1[1][13] = 5;S_Box1[1][14] = 3;S_Box1[1][15] = 8;S_Box1[2][0] = 4;S_Box1[2][1] = 1;S_Box1[2][2] = 14;S_Box1[2][3] = 8;S_Box1[2][4] = 13;S_Box1[2][5] = 6;S_Box1[2][6] = 2;S_Box1[2][7] = 11;S_Box1[2][8] = 15;S_Box1[2][9] = 12;S_Box1[2][10] = 9;S_Box1[2][11] = 7;S_Box1[2][12] = 3;S_Box1[2][13] = 10;S_Box1[2][14] = 5;S_Box1[2][15] = 0;S_Box1[3][0] = 15;S_Box1[3][1] = 12;S_Box1[3][2] = 8;S_Box1[3][3] = 2;S_Box1[3][4] = 4;S_Box1[3][5] = 9;S_Box1[3][6] = 1;S_Box1[3][7] = 7;S_Box1[3][8] = 5;S_Box1[3][9] = 11;S_Box1[3][10] = 3;S_Box1[3][11] = 14;S_Box1[3][12] = 10;S_Box1[3][13] = 0;S_Box1[3][14] = 6;S_Box1[3][15] = 13;S_Box2[0][0] = 15;
    S_Box2[0][1] = 1;S_Box2[0][2] = 8;S_Box2[0][3] = 14;S_Box2[0][4] = 6;S_Box2[0][5] = 11;S_Box2[0][6] = 3;S_Box2[0][7] = 4;S_Box2[0][8] = 9;S_Box2[0][9] = 7;S_Box2[0][10] = 2;S_Box2[0][11] = 13;S_Box2[0][12] = 12;S_Box2[0][13] = 0;S_Box2[0][14] = 5;S_Box2[0][15] = 10;S_Box2[1][0] = 3;S_Box2[1][1] = 13;S_Box2[1][2] = 4;S_Box2[1][3] = 7;S_Box2[1][4] = 15;S_Box2[1][5] = 2;S_Box2[1][6] = 8;S_Box2[1][7] = 14;S_Box2[1][8] = 12;S_Box2[1][9] = 0;S_Box2[1][10] = 1;S_Box2[1][11] = 10;S_Box2[1][12] = 6;S_Box2[1][13] = 9;S_Box2[1][14] = 11;S_Box2[1][15] = 5;S_Box2[2][0] = 0;S_Box2[2][1] = 14;S_Box2[2][2] = 7;S_Box2[2][3] = 11;S_Box2[2][4] = 10;S_Box2[2][5] = 4;S_Box2[2][6] = 13;S_Box2[2][7] = 1;S_Box2[2][8] = 5;S_Box2[2][9] = 8;S_Box2[2][10] = 12;S_Box2[2][11] = 6;S_Box2[2][12] = 9;S_Box2[2][13] = 3;S_Box2[2][14] = 2;S_Box2[2][15] = 15;S_Box2[3][0] = 13;S_Box2[3][1] = 8;S_Box2[3][2] = 10;S_Box2[3][3] = 1;S_Box2[3][4] = 3;S_Box2[3][5] = 15;S_Box2[3][6] = 4;S_Box2[3][7] = 2;S_Box2[3][8] = 11;S_Box2[3][9] = 6;S_Box2[3][10] = 7;S_Box2[3][11] = 12;S_Box2[3][12] = 0;S_Box2[3][13] = 5;S_Box2[3][14] = 14;S_Box2[3][15] = 9;
    S_Box3[0][0] = 10;S_Box3[0][1] = 0;S_Box3[0][2] = 9;S_Box3[0][3] = 14;S_Box3[0][4] = 6;S_Box3[0][5] = 3;S_Box3[0][6] = 15;S_Box3[0][7] = 5;S_Box3[0][8] = 1;S_Box3[0][9] = 13;S_Box3[0][10] = 12;S_Box3[0][11] = 7;S_Box3[0][12] = 11;S_Box3[0][13] = 4;S_Box3[0][14] = 2;S_Box3[0][15] = 8;S_Box3[1][0] = 13;S_Box3[1][1] = 7;S_Box3[1][2] = 0;S_Box3[1][3] = 9;S_Box3[1][4] = 3;S_Box3[1][5] = 4;S_Box3[1][6] = 6;S_Box3[1][7] = 10;S_Box3[1][8] = 2;S_Box3[1][9] = 8;S_Box3[1][10] = 5;S_Box3[1][11] = 14;S_Box3[1][12] = 12;S_Box3[1][13] = 11;S_Box3[1][14] = 15;S_Box3[1][15] = 1;S_Box3[2][0] = 13;S_Box3[2][1] = 6;S_Box3[2][2] = 4;S_Box3[2][3] = 9;S_Box3[2][4] = 8;S_Box3[2][5] = 15;S_Box3[2][6] = 3;S_Box3[2][7] = 0;S_Box3[2][8] = 11;S_Box3[2][9] = 1;S_Box3[2][10] = 2;S_Box3[2][11] = 12;S_Box3[2][12] = 5;S_Box3[2][13] = 10;S_Box3[2][14] = 14;S_Box3[2][15] = 7;S_Box3[3][0] = 1;S_Box3[3][1] = 10;S_Box3[3][2] = 13;S_Box3[3][3] = 0;S_Box3[3][4] = 6;S_Box3[3][5] = 9;S_Box3[3][6] = 8;S_Box3[3][7] = 7;S_Box3[3][8] = 4;S_Box3[3][9] = 15;S_Box3[3][10] = 14;S_Box3[3][11] = 3;S_Box3[3][12] = 11;S_Box3[3][13] = 5;S_Box3[3][14] = 2;S_Box3[3][15] = 12;
    S_Box4[0][0] = 7;S_Box4[0][1] = 13;S_Box4[0][2] = 14;S_Box4[0][3] = 3;S_Box4[0][4] = 0;S_Box4[0][5] = 6;S_Box4[0][6] = 9;S_Box4[0][7] = 10;S_Box4[0][8] = 1;S_Box4[0][9] = 2;S_Box4[0][10] = 8;S_Box4[0][11] = 5;S_Box4[0][12] = 11;S_Box4[0][13] = 12;S_Box4[0][14] = 4;S_Box4[0][15] = 15;S_Box4[1][0] = 13;S_Box4[1][1] = 8;S_Box4[1][2] = 11;S_Box4[1][3] = 5;S_Box4[1][4] = 6;S_Box4[1][5] = 15;S_Box4[1][6] = 0;S_Box4[1][7] = 3;S_Box4[1][8] = 4;S_Box4[1][9] = 7;S_Box4[1][10] = 2;S_Box4[1][11] = 12;S_Box4[1][12] = 1;S_Box4[1][13] = 10;S_Box4[1][14] = 14;S_Box4[1][15] = 9;S_Box4[2][0] = 10;S_Box4[2][1] = 6;S_Box4[2][2] = 9;S_Box4[2][3] = 0;S_Box4[2][4] = 12;S_Box4[2][5] = 11;S_Box4[2][6] = 7;S_Box4[2][7] = 13;S_Box4[2][8] = 15;S_Box4[2][9] = 1;S_Box4[2][10] = 3;S_Box4[2][11] = 14;S_Box4[2][12] = 5;S_Box4[2][13] = 2;S_Box4[2][14] = 8;S_Box4[2][15] = 4;S_Box4[3][0] = 3;S_Box4[3][1] = 15;S_Box4[3][2] = 0;S_Box4[3][3] = 6;S_Box4[3][4] = 10;S_Box4[3][5] = 1;S_Box4[3][6] = 13;S_Box4[3][7] = 8;S_Box4[3][8] = 9;S_Box4[3][9] = 4;S_Box4[3][10] = 5;S_Box4[3][11] = 11;S_Box4[3][12] = 12;S_Box4[3][13] = 7;S_Box4[3][14] = 2;S_Box4[3][15] = 14;
    S_Box5[0][0] = 2;S_Box5[0][1] = 12;S_Box5[0][2] = 4;S_Box5[0][3] = 1;S_Box5[0][4] = 7;S_Box5[0][5] = 10;S_Box5[0][6] = 11;S_Box5[0][7] = 6;S_Box5[0][8] = 8;S_Box5[0][9] = 5;S_Box5[0][10] = 3;S_Box5[0][11] = 15;S_Box5[0][12] = 13;S_Box5[0][13] = 0;S_Box5[0][14] = 14;S_Box5[0][15] = 9;S_Box5[1][0] = 14;S_Box5[1][1] = 11;S_Box5[1][2] = 2;S_Box5[1][3] = 12;S_Box5[1][4] = 4;S_Box5[1][5] = 7;S_Box5[1][6] = 13;S_Box5[1][7] = 1;S_Box5[1][8] = 5;S_Box5[1][9] = 0;S_Box5[1][10] = 15;S_Box5[1][11] = 10;S_Box5[1][12] = 3;S_Box5[1][13] = 9;S_Box5[1][14] = 8;S_Box5[1][15] = 6;S_Box5[2][0] = 4;S_Box5[2][1] = 2;S_Box5[2][2] = 1;S_Box5[2][3] = 11;S_Box5[2][4] = 10;S_Box5[2][5] = 13;S_Box5[2][6] = 7;S_Box5[2][7] = 8;S_Box5[2][8] = 15;S_Box5[2][9] = 9;S_Box5[2][10] = 12;S_Box5[2][11] = 5;S_Box5[2][12] = 6;S_Box5[2][13] = 3;S_Box5[2][14] = 0;S_Box5[2][15] = 14;S_Box5[3][0] = 11;S_Box5[3][1] = 8;S_Box5[3][2] = 12;S_Box5[3][3] = 7;S_Box5[3][4] = 1;S_Box5[3][5] = 14;S_Box5[3][6] = 2;S_Box5[3][7] = 13;S_Box5[3][8] = 6;S_Box5[3][9] = 15;S_Box5[3][10] = 0;S_Box5[3][11] = 9;S_Box5[3][12] = 10;S_Box5[3][13] = 4;S_Box5[3][14] = 5;S_Box5[3][15] = 3;
    S_Box6[0][0] = 12;S_Box6[0][1] = 1;S_Box6[0][2] = 10;S_Box6[0][3] = 15;S_Box6[0][4] = 9;S_Box6[0][5] = 2;S_Box6[0][6] = 6;S_Box6[0][7] = 8;S_Box6[0][8] = 0;S_Box6[0][9] = 13;S_Box6[0][10] = 3;S_Box6[0][11] = 4;S_Box6[0][12] = 14;S_Box6[0][13] = 7;S_Box6[0][14] = 5;S_Box6[0][15] = 11;S_Box6[1][0] = 10;S_Box6[1][1] = 15;S_Box6[1][2] = 4;S_Box6[1][3] = 2;S_Box6[1][4] = 7;S_Box6[1][5] = 12;S_Box6[1][6] = 9;S_Box6[1][7] = 5;S_Box6[1][8] = 6;S_Box6[1][9] = 1;S_Box6[1][10] = 13;S_Box6[1][11] = 14;S_Box6[1][12] = 0;S_Box6[1][13] = 11;S_Box6[1][14] = 3;S_Box6[1][15] = 8;S_Box6[2][0] = 9;S_Box6[2][1] = 14;S_Box6[2][2] = 15;S_Box6[2][3] = 5;S_Box6[2][4] = 2;S_Box6[2][5] = 8;S_Box6[2][6] = 12;S_Box6[2][7] = 3;S_Box6[2][8] = 7;S_Box6[2][9] = 0;S_Box6[2][10] = 4;S_Box6[2][11] = 10;S_Box6[2][12] = 1;S_Box6[2][13] = 13;S_Box6[2][14] = 11;S_Box6[2][15] = 6;S_Box6[3][0] = 4;S_Box6[3][1] = 3;S_Box6[3][2] = 2;S_Box6[3][3] = 12;S_Box6[3][4] = 9;S_Box6[3][5] = 5;S_Box6[3][6] = 15;S_Box6[3][7] = 10;S_Box6[3][8] = 11;S_Box6[3][9] = 14;S_Box6[3][10] = 1;S_Box6[3][11] = 7;S_Box6[3][12] = 6;S_Box6[3][13] = 0;S_Box6[3][14] = 8;S_Box6[3][15] = 13;
    S_Box7[0][0] = 4;S_Box7[0][1] = 11;S_Box7[0][2] = 2;S_Box7[0][3] = 14;S_Box7[0][4] = 15;S_Box7[0][5] = 0;S_Box7[0][6] = 8;S_Box7[0][7] = 13;S_Box7[0][8] = 3;S_Box7[0][9] = 12;S_Box7[0][10] = 9;S_Box7[0][11] = 7;S_Box7[0][12] = 5;S_Box7[0][13] = 10;S_Box7[0][14] = 6;S_Box7[0][15] = 1;S_Box7[1][0] = 13;S_Box7[1][1] = 0;S_Box7[1][2] = 11;S_Box7[1][3] = 7;S_Box7[1][4] = 4;S_Box7[1][5] = 9;S_Box7[1][6] = 1;S_Box7[1][7] = 10;S_Box7[1][8] = 14;S_Box7[1][9] = 3;S_Box7[1][10] = 5;S_Box7[1][11] = 12;S_Box7[1][12] = 2;S_Box7[1][13] = 15;S_Box7[1][14] = 8;S_Box7[1][15] = 6;S_Box7[2][0] = 1;S_Box7[2][1] = 4;S_Box7[2][2] = 11;S_Box7[2][3] = 13;S_Box7[2][4] = 12;S_Box7[2][5] = 3;S_Box7[2][6] = 7;S_Box7[2][7] = 14;S_Box7[2][8] = 10;S_Box7[2][9] = 15;S_Box7[2][10] = 6;S_Box7[2][11] = 8;S_Box7[2][12] = 0;S_Box7[2][13] = 5;S_Box7[2][14] = 9;S_Box7[2][15] = 2;S_Box7[3][0] = 6;S_Box7[3][1] = 11;S_Box7[3][2] = 13;S_Box7[3][3] = 8;S_Box7[3][4] = 1;S_Box7[3][5] = 4;S_Box7[3][6] = 10;S_Box7[3][7] = 7;S_Box7[3][8] = 9;S_Box7[3][9] = 5;S_Box7[3][10] = 0;S_Box7[3][11] = 15;S_Box7[3][12] = 14;S_Box7[3][13] = 2;S_Box7[3][14] = 3;S_Box7[3][15] = 12;
    S_Box8[0][0] = 13;S_Box8[0][1] = 2;S_Box8[0][2] = 8;S_Box8[0][3] = 4;S_Box8[0][4] = 6;S_Box8[0][5] = 15;S_Box8[0][6] = 11;S_Box8[0][7] = 1;S_Box8[0][8] = 10;S_Box8[0][9] = 9;S_Box8[0][10] = 3;S_Box8[0][11] = 14;S_Box8[0][12] = 5;S_Box8[0][13] = 0;S_Box8[0][14] = 12;S_Box8[0][15] = 7;S_Box8[1][0] = 1;S_Box8[1][1] = 15;S_Box8[1][2] = 13;S_Box8[1][3] = 8;S_Box8[1][4] = 10;S_Box8[1][5] = 3;S_Box8[1][6] = 7;S_Box8[1][7] = 4;S_Box8[1][8] = 12;S_Box8[1][9] = 5;S_Box8[1][10] = 6;S_Box8[1][11] = 11;S_Box8[1][12] = 0;S_Box8[1][13] = 14;S_Box8[1][14] = 9;S_Box8[1][15] = 2;S_Box8[2][0] = 7;S_Box8[2][1] = 11;S_Box8[2][2] = 4;S_Box8[2][3] = 1;S_Box8[2][4] = 9;S_Box8[2][5] = 12;S_Box8[2][6] = 14;S_Box8[2][7] = 2;S_Box8[2][8] = 0;S_Box8[2][9] = 6;S_Box8[2][10] = 10;S_Box8[2][11] = 13;S_Box8[2][12] = 15;S_Box8[2][13] = 3;S_Box8[2][14] = 5;S_Box8[2][15] = 8;S_Box8[3][0] = 2;S_Box8[3][1] = 1;S_Box8[3][2] = 14;S_Box8[3][3] = 7;S_Box8[3][4] = 4;S_Box8[3][5] = 10;S_Box8[3][6] = 8;S_Box8[3][7] = 13;S_Box8[3][8] = 15;S_Box8[3][9] = 12;S_Box8[3][10] = 9;S_Box8[3][11] = 0;S_Box8[3][12] = 3;S_Box8[3][13] = 5;S_Box8[3][14] = 6;S_Box8[3][15] = 11;

    data_out[4:1] = S_Box8[ { data_in[6] , data_in[1] } ][ data_in[5:2] ];
    data_out[8:5] = S_Box7[ { data_in[12] , data_in[7] } ][ data_in[11:8] ];
    data_out[12:9] = S_Box6[ { data_in[18] , data_in[13] } ][ data_in[17:14] ];
    data_out[16:13] = S_Box5[ { data_in[24] , data_in[19] } ][ data_in[23:20] ];
    data_out[20:17] = S_Box4[ { data_in[30] , data_in[25] } ][ data_in[29:26] ];
    data_out[24:21] = S_Box3[ { data_in[36] , data_in[31] } ][ data_in[35:32] ];    
    data_out[28:25] = S_Box2[ { data_in[42] , data_in[37] } ][ data_in[41:38] ];    
    data_out[32:29] = S_Box1[ { data_in[48] , data_in[43] } ][ data_in[47:44] ];    
         
    S_BOX=data_out;
      
    end
endfunction


function [32:1] Func_f(input [32:1] data_in, input [48:1] round_key);
    reg [48:1] sin;
    reg [32:1] sout;
    
    begin
      sin = round_key ^ shuffle_E(data_in);
      sout = S_BOX(sin);
      Func_f = shuffle_P(sout);

    end
endfunction

    reg [64:1] new_msg;
    reg [32:1] Left[16:0], Right[16:0];
    wire [48:1] key1, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12, key13, key14, key15, key16;
    reg [48:1] Key[16:1];
    integer i;
    KeyGen keygen(key ,key1, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12, key13, key14, key15, key16);

  always @(key16)
  begin
  
    Key[1] = key1; Key[2] = key2; Key[3] = key3; Key[4] = key4; Key[5] = key5; Key[6] = key6; Key[7] = key7; Key[8] = key8; Key[9] = key9; Key[10] = key10; Key[11] = key11; Key[12] = key12; Key[13] = key13; Key[14] = key14; Key[15] = key15; Key[16] = key16; 
    
    {Left[0], Right[0]} = shuffle_IP(plaintext);
    
    for(i=1; i<=16; i=i+1)
        begin
        Left[i]=Right[i-1];
        Right[i]=Left[i-1] ^ Func_f(Right[i-1], is_dec ? Key[17 - i] : Key[i]);
        end
    ciphertext[64:1] = shuffle_invIP({Right[16], Left[16]}); 
  end

endmodule

module test;
  
  //wire [48:1] key1, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12, key13, key14, key15, key16;
  //KeyGen pk(key,key1, key2, key3, key4, key5, key6, key7, key8, key9, key10, key11, key12, key13, key14, key15, key16);
      reg [64:1] key;
      reg [64:1] plaintext;
      wire [64:1] ciphertext;
           
      reg [64:1] enc_res;
      wire [64:1] dec_res;
  
      initial
      begin
        key = 64'b00010011_00110100_01010111_01111001_10011011_10111100_11011111_11110001;
        plaintext =  64'b0000_0001_0010_0011_0100_0101_0110_0111_1000_1001_1010_1011_1100_1101_1110_1111;
        enc_res = 64'b1000_0101_1110_1000_0001_0011_0101_0100_0000_1111_0000_1010_1011_0100_0000_0101;
      end
          
      DES enc(plaintext, key, 0, ciphertext);
      initial $monitor("enc=%b", ciphertext);
      
      DES dec(enc_res, key, 1, dec_res);
      initial $monitor("dec=%b", dec_res); 
endmodule