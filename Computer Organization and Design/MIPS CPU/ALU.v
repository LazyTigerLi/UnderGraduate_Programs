`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:51:04 03/28/2018 
// Design Name: 
// Module Name:    ALU 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ALU(
	 input [5:0] Opcode,
	 input [4:0] Shamt,
	 input [5:0] Funct,
  	 input [31:0] alu_a,
	 input [31:0] alu_b,
	 output reg [31:0] alu_out,
	 output reg gtz,
	 output reg ne,
	 output reg eq,
	 output reg gez,
	 output reg lez,
	 output reg ltz,
	 output reg carry					//溢出位
    );
	 
	 parameter R_type = 6'h0;	//add,addu,and,nor,or,sll,sllv,sra,srav,srl,srlv,sub,subu,xor,slt,sltu
	 parameter addi = 6'h8;
	 parameter addiu = 6'h9;
	 parameter andi = 6'hc;
	 parameter ori = 6'hd;
	 parameter xori = 6'he;
	 parameter slti = 6'ha;
	 parameter sltiu = 6'hb;
	 parameter bgtz = 6'h7;
	 parameter bne = 6'h5;
	 parameter j = 6'h2;
	 parameter jr = 6'h0;
	 parameter lw = 6'h23;
	 parameter sw = 6'h2b;
	 parameter beq = 6'h4;
	 parameter bgez = 6'h1;
	 parameter blez = 6'h6;
	 parameter bltz = 6'h1;
	 parameter clo_clz = 6'h1c;
	 parameter lui = 6'hf;
	 reg [32:0] temp1;
	 always @ (*)
	 begin
			carry = 1'b0;
			temp1 = 33'h0;
			if(Opcode == andi || (Opcode == R_type && Funct == 6'h24))alu_out = alu_a & alu_b;		//与，立即数与
			else if(Opcode == ori || (Opcode == R_type && Funct == 6'h25))alu_out = alu_a | alu_b;	//或，立即数或
			else if(Opcode == xori || (Opcode == R_type && Funct == 6'h26))alu_out = alu_a ^ alu_b;	//异或，立即数异或
			else if(Opcode == R_type && Funct == 6'h27)alu_out = ~(alu_a | alu_b);				//或非
			else if(Opcode == addiu || (Opcode == R_type && Funct == 6'h21)
					|| Opcode == lw || Opcode == sw)alu_out = alu_a + alu_b;
			//加，立即数加，无溢出位，取数，存数
			else if(Opcode == lui)
				begin
					alu_out[31:16] = alu_b[15:0];
					alu_out[15:0] = 16'h0;
				end
			else if(Opcode == clo_clz && Funct == 6'h21)alu_out = alu_a[0] + alu_a[1] + alu_a[2] + alu_a[3] + alu_a[4]
			 + alu_a[5] + alu_a[6] + alu_a[7] + alu_a[8] + alu_a[9] + alu_a[10] + alu_a[11] + alu_a[12] + alu_a[13] + alu_a[14]
			 + alu_a[15] + alu_a[16] + alu_a[17] + alu_a[18] + alu_a[19] + alu_a[20] + alu_a[21] + alu_a[22] + alu_a[23] + alu_a[24]
			 + alu_a[25] + alu_a[26] + alu_a[27] + alu_a[28] + alu_a[29] + alu_a[30] + alu_a[31];
			else if(Opcode == clo_clz && Funct == 6'h20)alu_out = 32'd32 - (alu_a[0] + alu_a[1] + alu_a[2] + alu_a[3] + alu_a[4]
			 + alu_a[5] + alu_a[6] + alu_a[7] + alu_a[8] + alu_a[9] + alu_a[10] + alu_a[11] + alu_a[12] + alu_a[13] + alu_a[14]
			 + alu_a[15] + alu_a[16] + alu_a[17] + alu_a[18] + alu_a[19] + alu_a[20] + alu_a[21] + alu_a[22] + alu_a[23] + alu_a[24]
			 + alu_a[25] + alu_a[26] + alu_a[27] + alu_a[28] + alu_a[29] + alu_a[30] + alu_a[31]);
			else if(Opcode == addi || (Opcode == R_type && Funct == 6'h20))
				begin temp1 = alu_a + alu_b;
						if(temp1[32] == 1'b1)
						begin
							alu_out = temp1[31:0];
							carry = 1'b1;
						end
						else
						begin
							alu_out = temp1[31:0];
							carry = 1'b0;
						end
				end
			//加，立即数加，有溢出位
			else if(Opcode == R_type && Funct == 6'h0)alu_out = alu_b << Shamt;			//逻辑左移
			else if(Opcode == R_type && Funct == 6'h4)alu_out = alu_b << alu_a;			//逻辑左移变量
			else if(Opcode == R_type && Funct == 6'h3)alu_out = alu_b >>> Shamt;			//算术右移
			else if(Opcode == R_type && Funct == 6'h7)alu_out = alu_b >>> alu_a;			//算术右移变量
			else if(Opcode == R_type && Funct == 6'h2)alu_out = alu_b >> Shamt;			//逻辑右移
			else if(Opcode == R_type && Funct == 6'h6)alu_out = alu_b >> alu_a;			//逻辑右移变量
			else if(Opcode == R_type && Funct == 6'h22)
				begin temp1 = alu_a - alu_b;															//减，有溢出位
					if(temp1[32] == 1'b1)
					begin
						alu_out = temp1[31:0];
						carry = 1'b1;
					end
					else
					begin
						alu_out = temp1[31:0];
						carry = 1'b0;
					end
				end
			else if(Opcode == bne || Opcode == bgtz || Opcode == beq || Opcode == bgez || Opcode == blez 
				|| Opcode == bltz || (Opcode == R_type && Funct == 6'h23))alu_out = alu_a - alu_b;			//减，无溢出位
			else if(Opcode == slti || (Opcode == R_type && Funct == 6'h2a))
				begin																			//小于则置位，有符号
					if(alu_a < alu_b)alu_out = 32'h1;
					else alu_out = 32'h0;
				end
			else if(Opcode == sltiu || (Opcode == R_type && Funct == 6'h2b))
				begin
					if(alu_a[31] < alu_b[31])alu_out = 32'h1;
					else if(alu_a[31] > alu_b[31])alu_out = 32'h0;
					else if({1'b0,alu_a[30:0]} < {1'b0,alu_b[30:0]})alu_out = 32'h1;
					else alu_out = 32'h0;
				end
			else alu_out = 32'h0;
	 end


	 always @ (*)					//比较大小好像是无符号的
	 begin
			if(alu_a[31] == 1'b0 && alu_a != 32'h0)gtz = 1'b1;
			else gtz = 1'b0;
	 end
	 always @ (*)
	 begin
			if(alu_a != alu_b)ne = 1'b1;
			else ne = 1'b0;
	 end
	 always @ (*)
	 begin
			if(alu_a[31] == 1'b1 || alu_a == 32'h0)lez = 1'b1;
			else lez = 1'b0;
	 end
	 always @ (*)
	 begin
			if(alu_a[31] == 1'b1)ltz = 1'b1;
			else ltz = 1'b0;
	 end
	 always @ (*)
	 begin
			if(alu_a[31] == 1'b0)gez = 1'b1;
			else gez = 1'b0;
	 end
	 always @ (*)
	 begin
			if(alu_a - alu_b == 32'h0)eq = 1'b1;
			else eq = 1'b0;
	 end
endmodule
