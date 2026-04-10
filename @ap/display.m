function display(ap_obj)
    arb_struct = struct(ap_obj);
    builtin ('display', [inputname(1), ' = ', mex_flint_interface(1001, arb_struct.idx, arb_struct.prec)]);
end
