`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   18:53:52 05/31/2018
// Design Name:   top
// Module Name:   D:/14.7/Programs_CPU/lab7_final/test.v
// Project Name:  lab7_final
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: top
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test;

	// Inputs
	reg clk;
	reg rst_n;
	reg [7:0] addr_sel;

	// Outputs
	wire [31:0] program_counter;
	wire [31:0] IR;
	wire [31:0] IR2;
	wire [3:0] sel_seg;
	wire [7:0] out_data;

	// Instantiate the Unit Under Test (UUT)
	top uut (
		.clk(clk), 
		.rst_n(rst_n), 
		.addr_sel(addr_sel), 
		.program_counter(program_counter), 
		.IR(IR), 
		.IR2(IR2), 
		.sel_seg(sel_seg), 
		.out_data(out_data)
	);

	integer i;
	initial begin
		// Initialize Inputs
		clk = 0;
		rst_n = 1;
		#5;
		rst_n = 0;
		#5;
		rst_n = 1;
		// Wait 100 ns for global reset to finish
		#100;
		
		for(i = 0; i < 600; i = i + 1)
		begin
			clk = ~clk;
			#5;
		end
	end
      
endmodule

