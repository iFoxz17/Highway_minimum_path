# Highway_minimum_path
**C project** to solve a linear variant of the **minimum path problem**.

## Description
Consider an **highway** containing several **stations**; each station stands at a given **distance** and can have an arbitrary number of **cars** with a fixed quantity of **fuel**, which allows the car to travel a certain distance. Given a **start station** and an **end station**, the goal is to find the **minimum number of stops** (and the **sequence of stations**) that allows to travel from the **starting station** to the **ending one**.

Formally, given:
 - A **set of stations** $S=$ \{ $s_1, s_2, ..., s_n$\};
 - A **function** $d : S \rightarrow \mathbb{N}$, that associates at every station his **distance**;
 - A **function** $c : S \rightarrow \mathbb{N}$, that associates at every station the car with the **max fuel**;

Find
 - $k \in N$, the **minimum number of stops** that allows to reach $s_n$ from $s_1$,
 - $M=$ \{ $s_1, s_\{i_1\}, s_\{i_2\}, ..., s_\{i_k\}, s_n$ \}, the **optimal sequence of stations**. 

## Commands
Five commands avaibles:
 - <code>***aggiungi-stazione*** *distance* *cars-number* *car-1* ... *car-n*</code>
   - Add a station to the highway, identified by <code>*distance*</code> and having <code>*cars-number*</code> vehicles. The fuels of each vehicle are listed after the number of vehicles. If a station at a given distance already exists, no insertion is performed.
   - **Expected output**: <code>aggiunta</code> / <code>non aggiunta</code>
 
 - <code>***demolisci-stazione*** *distance*</code>
   - Remove the station at the indicated <code>*distance*</code>, if exists.
   - **Expected output**: <code>demolita</code> / <code>non demolita</code>

 - <code>***aggiungi-auto*** *station-distance* *car-fuel*</code>
   - Add a car with the specified <code>*car-fuel*</code> to the station at the given <code>*station-distance*</code>, provided that the station exists. It is possible to have more than one car with the same fuel.
   - **Expected output**: <code>aggiunta / non aggiunta</code>

 - <code>***rottama-auto*** *station-distance* *car-fuel*</code>
   - Removes a car with the indicated <code>*car-fuel*</code> from the station at <code>*station-distance*</code>, if exists.
   - **Expected output**: <code>rottamata / non rottamata</code>
 
 - <code>***pianifica-percorso*** *start-station* *end-station*</code>
   - Plan the route with the fewest stops between <code>*start-station*</code> and <code>*end-station*</code>, assuming one exists. Stations are identified by their distance. It is possible to travel the highway backward just inserting <code>*start-station*</code> $>$ <code>end-station</code>.
   - **Expected output**:
     - If the **route exists**: ordered sequence of stations distances, including <code>*start-station*</code> and <code>*end-station*</code>.
     - If the **route does not exist**: <code>nessun-percorso</code>

## Usage
- **Compile** with the <code>make</code> tool
- **Receive** commands from <code>stdin</code>
- **Outputs** results on <code>stdout</code>
- Example using **BASH**: <code>cat \_commands_path\_ | ./main</code>

## Documentation
It is possible to generate <code>HTML</code> documentation for the class through the **doxygen tool**. To do so, just install <code>doxygen</code>, open the terminal in the project folder, and run the <code>doxygen</code> command. It will automatically search for the Doxyfile which is in the folder and create a new folder containing the newly generated documentation. To read it, just go into the folder and open <code>index.html</code> with your preferred browser.

## Tests
**Several tests** with relative expected **outputs** are avaible in the directory <code>test</code> as zip files; They can be run individually (using the Linux <code>diff</code> tool for checking outputs, for example) or all together; for this last option:
 - **Compile** the file <code>run_tests.c</code> with <code>gcc run_tests.c -o run_tests</code>;
 - **Extract** all the tests from compressed archive into the directory <code>test</code>;
 - **Run** the executable </code>run_tests</code> (<code>./run_tests</code>).

## Notes
For severals instances can be avaible **multiple optimal solutions**; as default is selected the solution which **minimizes** the **distances from** the **start** of the **highway** (both for **forward** or **backward route**), according to tests. This can be modified at **compile time** to **upgrade perfomances** (see module <code>solver</code> in the **documentation** for more details).
