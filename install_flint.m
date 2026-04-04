function install_flint (cmd)
% Install FLINT MEX interface.
%
%   'rebuild'  -  Rebuild and overwrite the MEX interface.
%

if (nargin < 1)
    cmd = '';
end

[flint_dir, ~, ~] = fileparts (mfilename ('fullpath'));

old_dir = cd (flint_dir);

if ispc ()
    msys64_path = fullfile('c:', 'msys64', 'mingw64');
end

if (strcmp (cmd, 'rebuild') || exist (['mex_flint_interface.', mexext()], 'file') ~= 3)

    cfiles = {fullfile(flint_dir, 'mex', 'mex_flint_interface.c'), ...
        fullfile(flint_dir, 'mex', 'mex_arb_t.c')};
    cflags = {'-Wall', '-Wextra', '-Imex'};
    ldflags = {'-lflint', '-lmpfr', '-lgmp'};

    if ispc ()
        setenv('MW_MINGW64_LOC', msys64_path)
        cflags = [cflags(:)', {['-I', fullfile(msys64_path, 'include')]}];
        ldflags = {['-L', fullfile(msys64_path, 'lib')], fullfile(msys64_path, 'lib', 'libflint.dll.a'), '-lmpfr', '-lgmp'};
    elseif ismac()
        [~, brew_path] = system ('brew --prefix');
        brew_path = deblank (brew_path);
        cflags = [cflags(:)', {['-I', fullfile(brew_path, 'include')]}];
        ldflags = [ldflags(:)', {['-L', fullfile(brew_path, 'lib')]}];
    end

    try
        if (exist('OCTAVE_VERSION', 'builtin') == 5)
            mex (cflags{:}, cfiles{:}, ldflags{:});
        else
            mex (['CFLAGS="$CFLAGS ', strjoin(cflags, ' '), '"'], cfiles{:}, ldflags{:});
        end
        if ispc ()
            movefile (['mex_flint_interface.', mexext()], fullfile(msys64_path, 'bin'), 'f');
        end
    catch exception
        cd (old_dir);
        error ('MEX interface creation failed: %s\n\nFLINT cannot be used.', exception.message);
    end

end

cd (old_dir);

add_to_path_if_not_exists (flint_dir);
if ispc ()
    add_to_path_if_not_exists (fullfile (msys64_path, 'bin'));
end

disp ('FLINT is ready to use.');
disp (['    Detected FLINT version: ', mex_flint_interface(22)]);
disp (['    Detected  MPFR version: ', mex_flint_interface(21)]);
disp (['    Detected   GMP version: ', mex_flint_interface(20)]);
end



function add_to_path_if_not_exists (p)

pc = regexp (path, pathsep, 'split');
if (ispc ())  % MS Windows is not case-sensitive
    bool = any (strcmpi (p, pc));
else
    bool = any (strcmp (p, pc));
end
if (~ bool)
    addpath (p);
end

end
