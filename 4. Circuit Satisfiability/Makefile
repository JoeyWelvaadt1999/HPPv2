all: circuitSatisfiability

circuitSatisfiability: circuitSatisfiability.cc
	mpic++ -O2 -o circuitSatisfiability circuitSatisfiability.cc

run: circuitSatisfiability
	mpiexec --oversubscribe -n $(or $(NP),4) ./circuitSatisfiability.exe