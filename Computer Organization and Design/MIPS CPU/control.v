`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:56:19 05/16/2018 
// Design Name: 
// Module Name:    control 
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
module control(
	input [5:0] Opcode,
	input [5:0] Funct,
	input rst_n,
	output reg RegDst,
	output reg ALUSrc,
	output reg MemWrite,
	output reg RegWrite,
	output reg MemtoReg,
	output reg Branch_gtz,
	output reg Branch_ne,
	output reg Branch_eq,
	output reg Branch_gez,
	output reg Branch_lez,
	output reg Branch_ltz,
	output reg [1:0] jump
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
	 always @ (*)
	 begin
			RegDst = 1'b0;ALUSrc = 1'b0;
			Branch_eq = 1'b0;Branch_gez = 1'b0;
			Branch_gtz = 1'b0;Branch_lez = 1'b0;
			Branch_ltz = 1'b0;Branch_ne = 1'b0;
			MemWrite = 1'b0;
			RegWrite = 1'b0;MemtoReg = 1'b0;
			jump = 2'b00;
			if(~rst_n)jump = 2'b00;
			if(Opcode == R_type)
				begin RegDst = 1'b1;ALUSrc = 1'b0;MemWrite = 1'b0;
						RegWrite = 1'b1;MemtoReg = 1'b0; end
			if(Opcode == clo_clz)
				begin RegDst = 1'b1;MemWrite = 1'b0;
						RegWrite = 1'b1;MemtoReg = 1'b0; end
			if(Opcode == addi || Opcode == addiu || Opcode == andi
				|| Opcode == ori || Opcode == xori || Opcode == slti || Opcode == sltiu)
				begin RegDst = 1'b0;ALUSrc = 1'b1; 
						MemtoReg = 1'b0;RegWrite = 1'b1; 
						MemWrite = 1'b0; end
			if(Opcode == lui)
				begin RegDst = 1'b0;MemWrite = 1'b0;ALUSrc = 1'b1;
						RegWrite = 1'b1;MemtoReg = 1'b0;end
			if(Opcode == lw)
				begin RegDst = 1'b0;ALUSrc = 1'b1;
						MemWrite = 1'b0;
						RegWrite = 1'b1;MemtoReg = 1'b1; end
						
			if(Opcode == sw)
				begin ALUSrc = 1'b1;MemWrite = 1'b1;
						RegWrite = 1'b0; end
						
			if(Opcode == bgtz || Opcode == bne || Opcode == beq || 
				Opcode == bgez || Opcode == blez || Opcode == bltz)
				begin ALUSrc = 1'b0;MemtoReg = 1'b1; 
						RegWrite = 1'b0;MemWrite = 1'b0;
						case(Opcode)
							bgtz:Branch_gtz = 1'b1;
							bne:Branch_ne = 1'b1;
							beq:Branch_eq = 1'b1;
							bgez:Branch_gez = 1'b1;
							blez:Branch_lez = 1'b1;
							bltz:Branch_ltz = 1'b1;
						endcase
				end
			if(Opcode == j)jump = 2'b01;
			if(Opcode == jr && Funct == 6'h8)jump = 2'b10;
	 end
endmodule
