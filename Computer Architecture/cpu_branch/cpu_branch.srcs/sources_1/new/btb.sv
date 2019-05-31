`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/05/30 15:14:05
// Design Name: 
// Module Name: btb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module btb #(
    parameter LOW_ADDR_LEN = 5,    //低位地址长度，低位地址用于在cache内寻址，也决定了cache的大小
    parameter TAG_LEN = 6
)(
    input clk,
    input rst,
    input wire [31:0] pc,
    output reg found,
    output reg [31:0] npc,
    //更新btb缓存的目标地址，也可能是删除项目
    input wire new_item,
    input wire [31:0] npc_update,
    input wire valid_update             //指示是将某个项目失效，还是写入新的项目
);
localparam PC_CNT = 1 << LOW_ADDR_LEN;
localparam UNUSED_ADDR_LEN = 30 - TAG_LEN - LOW_ADDR_LEN;

reg [TAG_LEN - 1 : 0] pc_tag [PC_CNT];
reg [31 : 0] pc_target [PC_CNT];
reg valid[PC_CNT];

reg [TAG_LEN - 1:0] tag;
reg [LOW_ADDR_LEN - 1:0] low_addr;
reg [1:0] word_addr;
reg [UNUSED_ADDR_LEN - 1:0] unused_addr;
assign {unused_addr,tag,low_addr,word_addr} = pc;

reg [31:0] pc_update;
reg [31:0] last_pc;

always @ (pc)
begin
    if(pc_tag[low_addr] == tag && valid[low_addr] == 1'b1)
    begin
        found = 1'b1;
        npc = pc_target[low_addr];
    end else found = 1'b0;
end

always @ (negedge clk or posedge rst)
begin
    if(rst)
    begin
        for(integer i = 0; i < PC_CNT; i++)
        begin
            pc_tag[i] = 0;
            pc_target[i] = 32'b0;
            valid[i] = 1'b0;
            pc_update = 32'b0;
            last_pc = 32'b0;
        end
    end
    else begin
        if(new_item)
        begin
            pc_tag[pc_update[1 + LOW_ADDR_LEN:2]] = pc_update[1 + LOW_ADDR_LEN + TAG_LEN:2 + LOW_ADDR_LEN];
            pc_target[pc_update[1 + LOW_ADDR_LEN:2]] = npc_update;
            valid[pc_update[1 + LOW_ADDR_LEN:2]] = 1'b1;
        end else if(!valid_update)
        begin
            valid[pc_update[1 + LOW_ADDR_LEN:2]] = 1'b0;
        end
        pc_update = last_pc;
        last_pc = pc;
    end
end

endmodule
