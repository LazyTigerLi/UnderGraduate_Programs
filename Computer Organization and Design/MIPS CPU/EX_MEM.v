`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:27:41 05/16/2018 
// Design Name: 
// Module Name:    EX_MEM 
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
module EX_MEM(
	input clk,
	input rst_n,
	input [31:0] new_pc,
	input [31:0] ALU_result,
	input [31:0] rData_reg2,
	input [4:0] wAddr_reg,
	input gtz,
	input ne,
	input eq,
	input gez,
	input lez,
	input ltz,
	
	input Branch_gtz,
	input Branch_ne,
	input Branch_eq,
	input Branch_gez,
	input Branch_lez,
	input Branch_ltz,
	
	input MemWrite,
	input RegWrite,
	input MemtoReg,
	input [4:0] Rt,
	
	output reg [31:0] new_pc_out,
	output reg [31:0] ALU_result_out,
	output reg [31:0] rData_reg2_out,
	output reg [4:0] wAddr_reg_out,
	output reg gtz_out,
	output reg ne_out,
	output reg eq_out,
	output reg gez_out,
	output reg lez_out,
	output reg ltz_out,
	
	output reg Branch_gtz_out,
	output reg Branch_ne_out,
	output reg Branch_eq_out,
	output reg Branch_gez_out,
	output reg Branch_lez_out,
	output reg Branch_ltz_out,
	
	output reg MemWrite_out,
	output reg RegWrite_out,
	output reg MemtoReg_out,
	output reg [4:0] Rt_out
    );
	 always @ (negedge clk or negedge rst_n)
	 begin
		if(~rst_n)
		begin
			new_pc_out <= 32'h0;
			ALU_result_out <= 32'h0;
			rData_reg2_out <= 32'h0;
			wAddr_reg_out <= 5'h0;
			gtz_out <= 1'b0;
			ne_out <= 1'b0;
			eq_out <= 1'b0;
			gez_out <= 1'b0;
			lez_out <= 1'b0;
			ltz_out <= 1'b0;
		
			Branch_eq_out <= 1'b0;Branch_gez_out <= 1'b0;
			Branch_gtz_out <= 1'b0;Branch_lez_out <= 1'b0;
			Branch_ltz_out <= 1'b0;Branch_ne_out <= 1'b0;
			MemWrite_out <= 1'b0;
			RegWrite_out <= 1'b0;
			MemtoReg_out <= 1'b0;
			Rt_out <= 5'b0;
		end
		else
		begin
			new_pc_out <= new_pc;
			ALU_result_out <= ALU_result;
			rData_reg2_out <= rData_reg2;
			wAddr_reg_out <= wAddr_reg;
			gtz_out <= gtz;
			ne_out <= ne;
			eq_out <= eq;
			gez_out <= gez;
			lez_out <= lez;
			ltz_out <= ltz;
			
			Branch_eq_out <= Branch_eq;Branch_gez_out <= Branch_gez;
			Branch_gtz_out <= Branch_gtz;Branch_lez_out <= Branch_lez;
			Branch_ltz_out <= Branch_ltz;Branch_ne_out <= Branch_ne;
			
			MemWrite_out <= MemWrite;
			RegWrite_out <= RegWrite;
			MemtoReg_out <= MemtoReg;
			Rt_out <= Rt;
		end
	 end
endmodule
