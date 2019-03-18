`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:35:11 10/21/2017 
// Design Name: 
// Module Name:    seg7_decoder 
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
module seg7_decoder(
	input [31:0] in_data,
	input clk,
	output reg [3:0] sel,
	output reg [7:0] out_data
    );
	reg [16:0] cnt = 17'd0;
	reg [28:0] cnt1 = 29'd0;
	reg flag = 1'b0;
	parameter intvl = 100000;
	reg [3:0] dat;
	reg [1:0] cur_sel;
	
	always @ (posedge clk)
	begin
		cnt <= cnt + 17'd1;
		cnt1 <= cnt1 + 29'd1;
		if(cnt == intvl)
		begin
			cnt <= 17'd0;
			cur_sel <= cur_sel + 2'b01;
		end
		if(cnt1 == 29'd100000000)
		begin
			cnt1 <= 29'h0;
			flag <= flag + 1'b1;
		end
	end
		
	always @ (*)
	begin
		sel <= 4'b0000;
		case(cur_sel)
			2'b00: begin 
						sel <= 4'b0111;
						if(flag == 1'b0)dat <= in_data[3:0];
						else dat <= in_data[19:16];
					 end
			2'b01: begin 
						sel <= 4'b1011;
						if(flag == 1'b0)dat <= in_data[7:4];
						else dat <= in_data[23:20];
					 end
			2'b10: begin
						sel <= 4'b1101; 
						if(flag == 1'b0)dat <= in_data[11:8];
						else dat <= in_data[27:24];
					 end
			2'b11: begin
						sel <= 4'b1110; 
						if(flag == 1'b0)dat <= in_data[15:12];
						else dat <= in_data[31:28];
					 end
		endcase
	end

	always @ (*)
	begin
		case(dat[3:0])
			4'b0000 : out_data <= 8'b00000011;
			4'b0001 : out_data <= 8'b10011111;
			4'b0010 : out_data <= 8'b00100101;
			4'b0011 : out_data <= 8'b00001101;
			4'b0100 : out_data <= 8'b10011001;
			4'b0101 : out_data <= 8'b01001001;
			4'b0110 : out_data <= 8'b01000001;
			4'b0111 : out_data <= 8'b00011011;
			4'b1000 : out_data <= 8'b00000001;
			4'b1001 : out_data <= 8'b00001001;
			4'b1010 : out_data <= 8'b11000001;
			4'b1100 : out_data <= 8'b01100011;
			4'b1101 : out_data <= 8'b10000101;
			4'b1110 : out_data <= 8'b01100001;
			4'b1111 : out_data <= 8'b01110001; 
			default: out_data <= 8'b00000001;
		endcase
	end
endmodule
