function disp(arb_obj)
  arb_struct = struct(arb_obj);
  builtin ('disp', mex_flint_interface(1001, arb_struct.idx, arb_struct.prec));
end