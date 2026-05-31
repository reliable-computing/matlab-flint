classdef ap < handle

    properties
        idx
        prec
    end

    methods(Static)
        function p = pi(tol)
            [n d] = rat(pi,tol);
            p = n/d;
        end
    end

    methods

        function obj = ap(str, prec)
            if ischar(str)
                obj.idx = mex_flint_interface(1000, str, prec);
            elseif isnumeric(str)
                obj.idx = str;
            end
            obj.prec = prec;
        end

        function delete(obj)
            mex_flint_interface(1002, obj.idx)
        end

        function disp(obj)
            builtin ('disp', mex_flint_interface(1001, obj.idx, obj.prec));
        end

        function display(obj)
            builtin ('display', [inputname(1), ' = ', mex_flint_interface(1001, obj.idx, obj.prec)]);
        end

        function c = plus(a, b)
            prec = min(a.prec, b.prec);
            idx = mex_flint_interface(1100, a.idx, b.idx, prec);
            c = ap(idx, prec);
        end

        function c = minus(a, b)
            prec = min(a.prec, b.prec);
            idx = mex_flint_interface(1101, a.idx, b.idx, prec);
            c = ap(idx, prec);
        end

        function c = times(a, b)
            prec = min(a.prec, b.prec);
            idx = mex_flint_interface(1102, a.idx, b.idx, prec);
            c = ap(idx, prec);
        end

        function c = rdivide(a, b)
            prec = min(a.prec, b.prec);
            idx = mex_flint_interface(1103, a.idx, b.idx, prec);
            c = ap(idx, prec);
        end

    end

end
