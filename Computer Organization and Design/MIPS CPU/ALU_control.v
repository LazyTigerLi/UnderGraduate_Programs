`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:11:46 05/16/2018 
// Design Name: 
// Module Name:    ALU_control 
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
module ALU_control(
	input [3:0] ALUOp,
	input [5:0] Funct,
	output reg [2:0] ALUControl
    );
	 always @ (Funct or ALUOp)
	 begin
			case(ALUOp)
				4'b0000: ALUControl = 3'b010;							//lw,sw,addi
				4'b0001: ALUControl = 3'b110;							//beq
				4'b0010: begin
							case(Funct)
								6'b100000:ALUControl = 3'b010;		//加
								6'b100010:ALUControl = 3'b110;		//减
								6'b100100:ALUControl = 3'b000;		//与
								6'b100101:ALUControl = 3'b001;		//或
								6'b101010:ALUControl = 3'b111;		//小于则置一
								6'b100110:ALUControl = 3'b100;		//异或
								6'b100111:ALUControl = 3'b011;		//或非
							endcase
						end
				4'b0011: ALUControl = 3'b000;							//andi
				4'b0100: ALUControl = 3'b001;							//ori
			endcase
	 end
endmodule
