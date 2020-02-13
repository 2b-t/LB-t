%% github.com/2b-t
%  December 2019

%% StepImport
%  Velocity and density import. Imports density as well as the velocities from 
%  *.bin-files placed in the output-folder of the format 'name.bin', ... .
%
%  arguments:
%  NX:   number of cells in x-direction
%  NY:   number of cells in y-direction
%  NZ:   number of cells in z-direction
%  NAME: name of current time step to be imported (e.g. macro_0)
%
%  return values:
%  rho: density - two- or three-dimensional matrix 
%  ux:  velocity in x-direction - three-dimensional matrix
%  uy:  velocity in y-direction - three-dimensional matrix
%  uz:  velocity in z-direction - three-dimensional matrix
%
function [rho, ux, uy, uz] = StepImport(NX, NY, NZ, NAME)
  
    dim = 4*NX*NY*NZ; %dimension of linear array in *.bin-file
    
    %% import from binary data
    fileID = fopen([pwd '/BIN/' NAME '.bin']); %open relevant file
    macro = fread(fileID, dim, 'double'); %import contiguous data
    fclose(fileID); %close file
	
    %% split up array into subarrays and reverse linear indexing: create matrix
    rho = reshape(macro(1:4:end), [NX, NY, NZ]); %reshape vectors to match dimensions
    ux  = reshape(macro(2:4:end), [NX, NY, NZ]);
    uy  = reshape(macro(3:4:end), [NX, NY, NZ]);
    uz  = reshape(macro(4:4:end), [NX, NY, NZ]);
end