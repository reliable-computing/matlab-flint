function c = times(a, b)
    a_struct = struct(a);
    b_struct = struct(b);
    prec = min(a_struct.prec, b_struct.prec);
    c_struct.idx = mex_flint_interface(1102, a_struct.idx, b_struct.idx, prec);
    c_struct.prec = prec;
    c_struct.cleanupObj = onCleanup(@() mex_flint_interface(1002, c_struct.idx));
    c = class(c_struct, 'ap');
end
