%% github.com/2b-t
%  December 2019

%% isOctave
%  Checks which environment is currently used.
%  Can be used to turn functions and features on and off depending on the environment.
%
%% Return values
%  oct_mat: boolean variable - true if the environment is Octave, false else (e.g. Matlab)
%
function oct_mat = isOctave()
  persistent cacheval;  % speeds up repeated calls

  if isempty (cacheval)
    cacheval = (exist ("OCTAVE_VERSION", "builtin") > 0);
  end

  oct_mat = cacheval;
end