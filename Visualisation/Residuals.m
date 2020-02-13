%% github.com/2b-t
%  December 2019

%% Calculates and plots pseudo-residuals
%  Calculates pseudo residuals by simply plotting the difference between
%  cells in between two different time steps in order to make sure the 
%  problem converges. These pseudo residuals are plotted similarily to the 
%  velocity field for ux as well as for uy as images.
%
%% Arguments:
%  fig: number of the figure where the residuals should be plotted
%  ux: velocity in what is displayed as horizontal x-direction
%  uy: velocity in what is displayed as vertical y-direction
%  ux_old: velocity from previous step in what is displayed as x-direction
%  uy_old: velocity from previous step in what is displayed as y-direction
%  steps: number of time steps in between two plot steps (for averaging)
%  width: dimension in x-direction
%  length: dimension in y-direction
%  PROFILE_CROSS_SECTION: logical value if cross_section should be plotted
%  x_cross: x-coordinate of cross-section (x=const)
%  y_cross: y-coordinate of cross-section (y=const)
%  BounceBack_x: bounce-back in cross-section in y-direction (x=const)
%  BounceBack_y: bounce-back in cross-section in x-direction (y=const)
%
function Residuals(fig, ux, uy, ux_old, uy_old, steps, ...
                   width, length, ...
                   PROFILE_CROSS_SECTION, x_cross, y_cross, ...
                   BounceBack_x, BounceBack_y)

    %% Check if figure was closed
    % if figure was closed: don't open again, simply don't do anything
    if (ishandle(fig) == 1) %only if figure is still open
        
        figure(fig); %set current figure
        
        %% Calculate changes of velocity field
        ux_change = (ux-ux_old)./steps; %difference velocity field from one time step to the next
        uy_change = (uy-uy_old)./steps;
        
        %% Plot cross-sections to figure with subplots
        success = false; %default variable for determining if plots whever two or four plots should
                    %be used(true: 2 plots only, false: 4 plots)
                    
        if (PROFILE_CROSS_SECTION == true)
        
            %% Y-cross-section subplot
            if ((x_cross > 0) && (x_cross <= width)) %only if valid input cross-sections
                subplot(2, 2, 1) %create 2x2 subplot, select subplot 1

                % Plot velocity over cross-section
                data = squeeze(ux(x_cross, :));
                plot(data, 1:length, 'Color', 'red'); %plot velocity over cross-section�
                hold on
                
                try
                    % Overlay walls
                    BW = 255*double(BounceBack_x); %convert to monochrome colour arrays (RGB) (~bbx for black)
                    BW(:, :, 2) = 255*double(BounceBack_x); %(~bbx for black)
                    BW(:, :, 3) = 255*double(BounceBack_x); %(~bbx for black)
                    image(-50, 0.5, BW, 'AlphaData', BounceBack_x) %plot with transparency map
                    line([0 0], [0 length], 'Color', 'red', 'LineStyle', '-.'); %plot reference line
                    title('Velocity $u_{h}$ in vertical cross-section')
                end
                
                %axis equal; %scale axis equally
                try
                    xlim([min(data) max(data)]);
                catch
                    xlim([-1 1]);
                end
                ylim([0 length]);
                set(gca, 'Layer', 'top') %plot axis on top of image
                set(gca, 'yticklabel', []) %hide tick labels
                pbaspect([width length 1]) %set aspect ratio
                success = true; %set success variable
                hold off
            end


            %% X-cross-section subplot
            if ((y_cross > 0) && (y_cross <= length)) %only if valid input
                subplot(2,2,3) %select subplot 3

                % Plot velocity over cross-section
                data = squeeze(uy(:, y_cross)); %data to be plotted
                plot(1:width, data, 'Color', 'red'); %plot velocity over cross-section
                hold on

                % Overlay walls
                try
                    BW = 255*double(BounceBack_y); %convert to monochrome colour arrays (RGB) (~bby for black)
                    BW(:, :, 2) = 255*double(BounceBack_y); %(~bby for black)
                    BW(:, :, 3) = 255*double(BounceBack_y); %(~bby for black)
                    image(0.5, -50, BW, 'AlphaData', BounceBack_y) %plot with transparency map
                    line([0 width], [0 0], 'Color', 'red', 'LineStyle', '-.'); %plot reference line
                    title('Velocity $u_{v}$ in horizontal cross-section')
                end
                
                %axis equal; %scale axis equally
                xlim([0 width]); %set axis limits
                try
                    ylim([min(data) max(data)]);
                catch
                    ylim([-1 1]);
                end
                set(gca, 'Layer', 'top') %plot axis on top of image
                set(gca, 'xticklabel', []) %hide tick labels
                pbaspect([width length 1]) %set aspect ratio
                success = true; %set success variable
                hold off
            end
            
        end
        
        
        %% Plot velocity field changes to figure with subplots
        
        %% UX subplot
        % Select correct subfigure and plot
        if (success == true)
            subplot(2,2,2) %select subplot 2 (2x2)
        else
            subplot(2,1,1) %select subplot 1 (2x1)
        end
        imagesc(flipud(ux_change(:,length:-1:1)')); %image plot where colour represents scaled velocity
        
        % Format plot
        colorbar('eastoutside') %show colorbar on left hand side (for velocity scaling)
        set(gca, 'xtick',[]); %hide ticks
        set(gca,'ytick', []);
        set(gca, 'xticklabel', []); %hide labels
        set(gca, 'yticklabel', []);
        title('Change of velocity $\Delta u_{h}$')
        axis equal; %scale axis equally
        axis([0.5 0.5+width 0.5 0.5+length]); %set axis limits (compensate 0.5 default offset)
        set(gca, 'YDir', 'normal') %y-axis direction normal instead of reverse
        
        
        %% UY subplot
        % Select correct subfigure and plot
        if (success == true)
            subplot(2,2,4) %select subplot 4 (2x2)
        else
            subplot(2,1,2) %select subplot 2 (2x1)
        end
        imagesc(flipud(uy_change(:,length:-1:1)')); %image plot where colour represents scaled velocity

        % Format plot
        colorbar('eastoutside'); %show colorbar on left hand side (for velocity scaling)
        set(gca, 'xtick', []); %hide ticks
        set(gca, 'ytick', []);
        set(gca, 'xticklabel', []); %hide labels
        set(gca, 'yticklabel', []);
        title('Change of velocity $\Delta u_{v}$')
        axis equal; %scale axis equally
        axis([0.5 0.5+width 0.5 0.5+length]); %set axis limits (compensate 0.5 default offset)
        set(gca, 'YDir', 'normal') %y-axis direction normal instead of reverse
        drawnow %complete drawing

    end
    
end