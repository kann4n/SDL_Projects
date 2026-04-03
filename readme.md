<table>
    <tr>
        <td>
            <h3>Fluid</h3>
            <img src="imgs/fluid-v1.png" width="400"/>
            <p>
                <strong>Mechanism</strong><br>
                A grid-based cellular automata simulation focusing on fluid dynamics.
                <ul>
                    <li><strong>Grid System:</strong> The screen is divided into a <code>CELL_SIZE</code> grid of 'Fluid', 'Wall', or 'Empty' cells.</li>
                    <li><strong>Physics:</strong> Implements a falling-sand style algorithm where fluid cells check the space below and move if it is empty.</li>
                    <li><strong>Interaction:</strong> Users can paint walls (W), spawn fluid (F), or erase (E) in real-time.</li>
                </ul>
            </p>
            <p>
                <strong>Compile</strong><br>
                <code>cd fluid && gcc fluid.c -o fluid -lSDL3</code>
            </p>
            <p>
                <strong>Run</strong><br>
                <code>./fluid</code>
            </p>
        </td>
        <td>
            <h3>Random Walk</h3>
            <img src="imgs/random-walk-v1.png" width="400"/>
            <p>
                <strong>Mechanism</strong><br>
                A multi-agent simulation where each "agent" performs a stochastic path.
                <ul>
                    <li><strong>Color:</strong> Uses HSL-to-RGB conversion to give each agent a unique hue based on its index.</li>
                    <li><strong>Movement:</strong> Supports 4-directional or 8-directional paths constrained by window boundaries.</li>
                    <li><strong>Scale:</strong> A <code>SCALE</code> constant defines the step length, allowing for solid lines or dotted patterns.</li>
                </ul>
            </p>
            <p>
                <strong>Compile</strong><br>
                <code>cd random_walk && gcc random_walk.c -o random_walk -lSDL3 -lm</code>
            </p>
            <p>
                <strong>Run</strong><br>
                <code>./random_walk &lt;num_of_agent&gt;</code>
            </p>
        </td>
    </tr>
    <tr>
        <td>
            <h3>Space Sim</h3>
            <img src="imgs/space-sim-v0.1.png" width="400"/>
            <p>
                <strong>Mechanism</strong><br>
                A 2D orbital mechanics simulation using parent-child relationships.
                <ul>
                    <li><strong>Hierarchical Orbits:</strong> Bodies can orbit other bodies (e.g., Moon orbits Earth, Earth orbits Sun) using pointer references.</li>
                    <li><strong>Trigonometry:</strong> Uses <code>cosf</code> and <code>sinf</code> with <code>delta_time</code> to calculate precise circular positions.</li>
                    <li><strong>Visuals:</strong> Renders orbital paths as geometric hints and supports rotated textures for planetary axial rotation.</li>
                </ul>
            </p>
            <p>
                <strong>Compile</strong><br>
                <code>cd space_sim && gcc space_sim.c -o space_sim -lSDL3 -lSDL3_image -lm</code>
            </p>
            <p>
                <strong>Run</strong><br>
                <code>./space_sim</code>
            </p>
        </td>
        <td>
            <h3>Digital Clock</h3>
            <img src="imgs/digital-clock.png" width="400"/>
            <p>
                <strong>Mechanism</strong><br>
                A real-time digital clock rendered using 7-segment display logic.
                <ul>
                    <li><strong>7-Segment Display:</strong> Each digit is drawn by selectively filling horizontal and vertical segments based on the digit value.</li>
                    <li><strong>Real-Time:</strong> Reads the current system time each frame to update the display of hours, minutes, and seconds.</li>
                    <li><strong>Colons:</strong> Blinking colon separators are rendered between the hour, minute, and second digit pairs.</li>
                </ul>
            </p>
            <p>
                <strong>Compile</strong><br>
                <code>cd digital_clock && gcc main.c clock.c -o digital_clock -lSDL3</code>
            </p>
            <p>
                <strong>Run</strong><br>
                <code>./digital_clock</code>
            </p>
        </td>
    </tr>
</table>
