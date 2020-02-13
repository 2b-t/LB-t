%% github.com/2b-t
%  December 2019

%% LBM: visualisation
%  Visualisation of the velocity field in a adequate grid using a colored
%  image, streamlines and quiver (iso-density lines deactivated).

%% Arguments:
%  fig: number of figure where plot should be generated
%  res: number of figure where residuals are plotted
%  RHO0: the initial density
%  ux: velocity cross-section in what is displayed as horizontal x-direction
%  uy: velocity cross-section in what is displayed as vertical y-direction
%  uz: velocity cross-section in direction that won't be displayed (needed 
%      for velocity magnitude)
%  rho: density (connected to pressure via the speed of sound cs)
%  BounceBack_cross: a logical matrix that determines where a boundary is 
%                    present in the current cross-section
%  xx: mesh-grid in x-direction for visualisation
%  yy: mesh-grid in x-direction for visualisation
%  width: dimension of the cross-section in what is displayed as x-direction
%  height: dimension of the cross-section in what is displayed as y-direction
%  STREAMLINES: logical argument that determines if streamlines are plotted
%               (true) or not (false)
%  VORTICITY: logical argument that determines if iso-vorticity lines is 
%             plotted (true) or not (false)
%  PRESSURE: logical argument that determines if iso-pressure lines are 
%            plotted (true) or not (false)
%  CBAR_SCALE: scale of the color bar, either 'auto' for automatical or
%              [min max] for a custom scale
%  LABELS: turn axis labels on (true) and off (false)
%  PROFILE_CROSS_SECTION: show velocity cross-section in velocity magnitude
%                         plot (true/false)
%  x_cross: coordinates of x-cross-section
%  y_cross: coordinates of y-cross-section
%
function Visualisation(fig, res, RHO0, ux, uy, uz, rho, BounceBack_cross, xx, yy, ...
                       width, height, STREAMLINES, VORTICITY, PRESSURE, ...
                       CBAR_SCALE, LABELS, ...
                       PROFILE_CROSS_SECTION, x_cross, y_cross)
    
    if (ishandle(res) == 1) %if other figure is open
        figure(fig); %choose figure to plot on
    end

	%% Plot velocity magnitude colour map
    clf; %clear current figure
    %u = reshape(sqrt(ux.^2+uy.^2), width, height); %amount of velocity vector
    u = reshape(sqrt(ux.^2+uy.^2+uz.^2), width, height);
    imagesc(flipud(u(:,height:-1:1)')); %image plot where colour represents scaled velocity
    hold on %continue plotting in current figure
    
	%% Plot streamlines/quiver
	if (STREAMLINES == true) %only if wished
        try
            %s = quiver(xx, yy, squeeze(ux)', squeeze(uy)', 'k'); %quiver (smaller arrows but information about amount of vector)
            [verts, averts] = streamslice(xx, yy, squeeze(ux)', squeeze(uy)', 2); %streamlines (just direction)
            s = streamline([verts averts]); %draw arrows
            set(s, 'Color', 'k') %set streamline color to black
        catch
           %catch error is streamlines point into plane 
        end
	end
    
    %% Plot 2D iso-vorticity lines
    %The vorticity of a vector field is given by (grad x u) and hence
    %simplifies to a scalar w = (dv_y/dx-dv_x/dy) in case of a 2D vector
    %field. This can be discretised to w_d=(v_y_(i+1) - v_y_(i-1))/(2 dx) -
    %(v_x_(i+1) - v_x_(i-1))/(2 dy) and as in LBM a reglar cartesian grid
    % is used (dx=dy) w_d ~ v_y_(i+1) - v_y_(i-1) -v_x_(i+1) + v_x_(i-1).
	if (VORTICITY == true) %only if wished
        wy = circshift(uy(:,:), [1,0]) - circshift(uy(:,:), [-1,0]); %vorticity due to y-velocity
        wx = circshift(ux(:,:), [0,1]) - circshift(ux(:,:), [0,-1]); %vorticity due to x-velocity
        w = wy - wx; %overall vorticity
        %imagesc(flipud(w(:,height:-1:1)')); %plot iso-vorticity lines as colour map
        [~, iso_vor] = contour(squeeze(w)', 100, ':'); %plot iso-vorticity lines
        set(iso_vor, 'LineColor', 'black')  %set iso-vorcity color to black
        wp = w; %all positive values of w
        wp(w < 0) = nan; %replace all negative values with NaN
        wn = w; %all negative values of w
        wn(w >= 0) = nan; %replace all positive values with NaN
        [~, iso_vor_p] = contour(squeeze(wp)', 20, '--'); %plot positive iso-vorticity lines
        [~, iso_vor_n] = contour(squeeze(wn)', 20); %plot negative iso-vorticity lines
        set(iso_vor_p, 'LineColor', 'black')  %set iso-vorcity color to black
        set(iso_vor_n, 'LineColor', 'black')  %set iso-vorcity color to black
	end
            
	%% Plot iso-density lines (iso-pressure for D2Q9 and D3Q19 models = rho/3)
    cs = 1/sqrt(3); %lattice speed of sound
    if (PRESSURE == true) %only if wished
        %[C,h] = contour(squeeze((rho-1)/3)', 25); %plot iso-density lines
        %clabel(C, h); %label iso-density levels
        rho_p = rho; %positive pressure
        rho_p(rho < 1) = nan;
        rho_n = rho; %negative pressure
        rho_n(rho > 1) = nan;
        contour((squeeze(rho_p)' - RHO0)*cs^2, 30, 'k'); %plot iso-density lines (positive)
        contour((squeeze(rho_n)' - RHO0)*cs^2, 30, '--k'); %plot iso-density lines (negative)
        %n_p =  logspace(-3, -5, 20);
        %n_n = -logspace(-3, -5, 20);
        %contour((squeeze(rho_p)' - rho0)*cs^2, n_p, 'k'); %plot iso-density lines (positive)
        %contour((squeeze(rho_n)' - rho0)*cs^2, n_n, '--k'); %plot iso-density lines (negative)
    end
    
    %% Overlay walls
    try
        BW = 255*double(~BounceBack_cross); %convert to monochrome colour arrays (RGB)
        BW(:, :, 2) = 255*double(~BounceBack_cross);
        BW(:, :, 3) = 255*double(~BounceBack_cross);
    
        image(BW, 'AlphaData', BounceBack_cross) %plot with transparency map
    end
    
    %% Overlay cross-section markers
    if (PROFILE_CROSS_SECTION == true)
        line([x_cross x_cross], [0 height], 'Color', 'red', 'LineStyle', '-.'); %plot reference lines
        line([0 width], [y_cross y_cross], 'Color', 'red', 'LineStyle', '-.');
    end
            
	  %% Formatting
    if (strcmp(CBAR_SCALE,'auto') == 1) %if scale set to auto
        try
            caxis([0, max(max(u))]); %set maximum value to current maximum
        catch
            caxis([0, 1]); %set maximum value to current maximum
        end
    else
        caxis(CBAR_SCALE); %scale should be a vector with two entries
    end
    
    colorbar('westoutside') %show colorbar on left hand side (for velocity scaling)
    if (LABELS == true)
    	grid on %show grid
    	grid minor %show minor grid
    else
    	set(gca, 'XTick', [], 'YTick', []); %remove ticks
    	set(gca, 'XTickLabel', [], 'YTickLabel', []); %remove tick labels
    	grid off %remove grid         
    end   
    if (isOctave == false) 
	    axis equal; %scale axis equally
    else
      axis("equal")
    end
	  axis([0.5 0.5+width 0.5 0.5+height]); %set axis limits (compensate 0.5 default offset)
	  set(gca,'YDir','normal') %y-axis direction normal instead of reverse
    
    %% Settings for export with minimal borders
    %axis off;  %do not show axis
    %set(gcf,'color','w');  %set background color to white
    %ax = gca;  %minimise border around image in figure
    %outerpos = ax.OuterPosition;
    %ti = ax.TightInset; 
    %left = outerpos(1) + ti(1);
    %bottom = outerpos(2) + ti(2);
    %ax_width = outerpos(3) - ti(1) - ti(3);
    %ax_height = outerpos(4) - ti(2) - ti(4);
    %ax.Position = [left bottom ax_width ax_height];
    
	  drawnow %complete drawing

end