// for P=10000000, tic, p=P*3; x=ap('1',p), s=ap('1',p), t=s; for k=1:100, t=t.*x./ap(int2str(k),p); s=s+t; end, s,toc, end

// gcc -Wall -Wextra -o iter.exe iter.c -lflint && /usr/bin/time -v ./iter.exe > iter_output.txt

#include "flint/arb.h"


int main (void)
{
    slong prec = 10000000*3;

    arb_t s, t;

    arb_init(s);
    arb_init(t);

    arb_set_str(s, "1", prec);
    arb_set (t, s);

    for (ulong k = 1; k <= 100; k++)
    {
        arb_div_ui (t, t, k, prec);
        arb_add (s, s, t, prec);
    }

    char *output_buf = arb_get_str (s, prec, 0);
    //printf("%s\n", output_buf);
    
    arb_clear(s);
    arb_clear(t);

    return 0;
}
