function val = ap(str, prec)
    val.idx = mex_flint_interface(1000, str, prec);
    val.prec = prec;
    val.cleanupObj = onCleanup(@() mex_flint_interface(1002, val.idx));
    val = class(val,'ap');
end
