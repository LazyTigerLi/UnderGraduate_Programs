`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:23:44 05/16/2018 
// Design Name: 
// Module Name:    ID_EX 
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
module ID_EX(
	input clk,
	input rst_n,
	input [31:0] PC,
	input [31:0] instruction,
	input [31:0] rData1_reg,
	input [31:0] rData2_reg,
	
	input RegDst,
	input ALUSrc,
	input Branch_gtz,
	input Branch_ne,
	input Branch_eq,
	input Branch_gez,
	input Branch_lez,
	input Branch_ltz,
	
	input MemWrite,
	input RegWrite,
	input MemtoReg,
	
	output reg [31:0] PC_out,
	output reg [31:0] instruction_out,
	output reg [31:0] rData1_reg_out,
	output reg [31:0] rData2_reg_out,
	
	output reg RegDst_out,
	output reg ALUSrc_out,
	output reg Branch_gtz_out,
	output reg Branch_ne_out,
	output reg Branch_eq_out,
	output reg Branch_gez_out,
	output reg Branch_lez_out,
	output reg Branch_ltz_out,
	output reg MemWrite_out,
	output reg RegWrite_out,
	output reg MemtoReg_out
    );
	 always @ (negedge clk or negedge rst_n)
	 begin
		if(~rst_n)
		begin
			PC_out <= 32'h0;
			instruction_out <= 32'h0;
			rData1_reg_out <=  32'h0;
			rData2_reg_out <= 32'h0;
		
			RegDst_out <= 1'b0;
			ALUSrc_out <= 1'b0;
			Branch_eq_out <= 1'b0;Branch_gez_out <= 1'b0;
			Branch_gtz_out <= 1'b0;Branch_lez_out <= 1'b0;
			Branch_ltz_out <= 1'b0;Branch_ne_out <= 1'b0;
			MemWrite_out <= 1'b0;
			RegWrite_out <= 1'b0;
			MemtoReg_out <= 1'b0;
		end
		else
		begin
			PC_out <= PC;
			instruction_out <= instruction;
			rData1_reg_out <= rData1_reg;
			rData2_reg_out <= rData2_reg;
			
			RegDst_out <= RegDst;
			ALUSrc_out <= ALUSrc;
			
			Branch_eq_out <= Branch_eq;Branch_gez_out <= Branch_gez;
			Branch_gtz_out <= Branch_gtz;Branch_lez_out <= Branch_lez;
			Branch_ltz_out <= Branch_ltz;Branch_ne_out <= Branch_ne;
			
			MemWrite_out <= MemWrite;
			RegWrite_out <= RegWrite;
			MemtoReg_out <= MemtoReg;	
		end
	 end
endmodule
