// genesis

object	isynchan        Synchan_type ISynchan segment channel synchannel              \
	-author 	"Mike Vanier 11/94"                                          \ 
	-actions 	CREATE INIT PROCESS HPROCESS RESET CHECK RECALC SAVE2 RESTORE2 EVENT  \
			ADDMSGIN DELETEMSGIN MSGINDELETED RESETBUFFER DELETE COPY SET SHOW    \
	-messages 	VOLTAGE 	0 	1 Vm                                 \	
			ACTIVATION 	1 	1 activation                         \
			RAND_ACTIVATION 2 	2 probability amplitude              \
			MOD		3	1 modulation                         \
			SPIKE	       -1	0                                    \
	-readonly       activation            "driving force for channel conductance"                    \
	-readwrite		Ik                    "channel current"                                          \
	-readwrite		Gk                    "conductance of channel"                                   \
	-readwrite		Ek                    "reversal potential of channel"                            \
	-readwrite		X                     "internal variable for implementing synaptic conductance"  \
	-readwrite		Y                     "internal variable for implementing synaptic conductance"  \
	-hidden		xconst1               "internal variable for synaptic conductances"              \
	-hidden		yconst1               "internal variable for synaptic conductances"              \
	-hidden		xconst2               "internal variable for synaptic conductances"              \
	-hidden		yconst2               "internal variable for synaptic conductances"              \
	-hidden		norm                  "normalization factor for psp amplitude"                   \
	-readwrite	tau1                  "first time constant of channel activation"                \
	-readwrite	tau2                  "not used"               \
	-readwrite	gmax                  "peak conductance"                                         \
	-readwrite	frequency             "random activation frequency" 0.0                          \
	-readonly	nsynapses             "Number of incoming spike messages"                        \
	-hidden		synapse_size          "size of synapse in bytes"                                 \
	-readonly	event_buffer_size     "size of event buffer" 0                                   \
	-readonly       pending_events        "number of pending spike events in buffer" 0               \
	-hidden         FreeSynapticEvents    "free list of synaptic event nodes"                        \
	-hidden		PendingSynapticEvents "list of pending synaptic events"                          \
	-readwrite      nodes_per_synapse     "number of event nodes to allocate per synapse" 1.0        \
	-hidden         list_alloced          "flag for whether the freelist has been allocated" 0       \
        -hidden         allocednodes          "number of nodes that are the head of a malloced block" 0  \
	-readwrite	synapse               "synapse buffer"                                           \   
	-description "Synaptically activated channel with instantaneous rise," \
                     "and exponential decay with time constant tau1."
