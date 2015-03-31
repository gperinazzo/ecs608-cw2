#!/bin/bash

# Processes simultaneously
N=4				# 8
# Number of times each case is executed
R=10            # 100
# Minimum, maximum and step for number of events
MIN_E=128       # 512
MAX_E=1024      # 4096
STEP_E=64       # 256
# Minimum and maximum for number of 'threads' (threads increase exponentially)
MIN_T=2
MAX_T=16        # 64
# Message number will vary from 0 to the half of number of events Always with
# num_m different values (step will be range/num_m)
# Make sure its always a multiple of N
NUM_M=4         # 16

# Auxiliary values
((_N=$N - 1))
((ROUNDS=$NUM_M / $N - 1))

# Background process id and arguments array
declare -a PIDS
declare -a ARGS

NUM_E=128

for NUM_E in `seq $MIN_E $STEP_E $MAX_E`
do
	NUM_T=$MIN_T
	while [[ $NUM_T -le $MAX_T ]]
	do
		# Compute current step
		((M_STEP=($NUM_E / 2) / $NUM_M))

		ARGS[0]=$NUM_E  # events
		ARGS[1]=$NUM_T  # threads

		for i in `seq 0 $ROUNDS`
		do
			# This round initial message number
			((ARGS[2]=$i * $N * $M_STEP ))

			# Run processes in background
			for _i in `seq 0 $_N`
			do
				# Update Message number for current execution
				((ARGS[2]+=$M_STEP))

				# Launch processes
				echo ${ARGS[@]} ">" ${ARGS[0]}.${ARGS[1]}.${ARGS[2]}.out
				./run_aux.sh ${ARGS[@]} $R > ${ARGS[0]}.${ARGS[1]}.${ARGS[2]}.out &
				PIDS[$_i]=$!

			done

			# Wait for processes
			echo Waiting for PIDs ${PIDS[@]}...
			wait ${PIDS[@]}
		done

		# Increase thread number
		((NUM_T=$NUM_T * 2))
	done
done
