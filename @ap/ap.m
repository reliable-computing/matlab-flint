function val = ap(str, prec)
    val.idx = mex_flint_interface(1000, str, prec);
    val.prec = prec;
    val = class(val,'ap');
end
