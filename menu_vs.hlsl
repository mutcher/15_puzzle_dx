#include "menu_inc.hlsli"

psInput main(vsInput input)
{
    psInput output;
    output.pos = input.pos;
    output.tex = input.tex;

	return output;
}