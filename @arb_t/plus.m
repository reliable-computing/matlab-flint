function c = plus(a, b)
    a_struct = struct(a);
    b_struct = struct(b);
    prec = min(a_struct.prec, b_struct.prec);
    c_struct.idx = mex_flint_interface(1100, a_struct.idx, b_struct.idx, prec);
    c_struct.prec = prec;
    c = class(c_struct, 'arb_t');
end