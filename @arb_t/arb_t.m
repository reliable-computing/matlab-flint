function val = arb_t(str, prec)
  val.idx = mex_flint_interface(1000, str, prec);
  val.prec = prec;
  val = class(val,'arb_t');
end