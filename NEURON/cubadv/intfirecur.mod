: dm/dt = (minf - m)/tau
: input event adds w to m
: when m = 1, or event makes m >= 1 cell fires

NEURON {
	ARTIFICIAL_CELL IntFireCur
	RANGE tau, m, refrac, minf, gid
	: m plays the role of voltage
}

PARAMETER {
	tau = 5 (ms)   <1e-9,1e9>
	refrac = 5 (ms) <0,1e9>
	minf = 0
	gid = -1
}

ASSIGNED {
	m
	t0(ms)
	on
	cnt
}

INITIAL {
	m = 0
	t0 = t
	on = 1 : 0 means in refractory period
	net_send(firetime(), 1)
}

FUNCTION M() {
	if (on) {
		M = minf + (m - minf)*exp(-(t - t0)/tau)
	}else{
		M = 0
	}
}

NET_RECEIVE (w) {
	m = M()
	t0 = t
	if (flag == 1) { : try to regularize if several exc and inhib events at same time
		if (m >= .999999) { : fire if m above 1 - eps
			net_event(t)
			m = 0
			on = 0 : go into refractory mode
			net_send(refrac + 1e-6, 2) : kind of a hacky way to avoid events at same time which may be handled before or after depending on cvode.queue_mode details
		}else{
			net_send(firetime(), 1)
		}
	}else if (flag == 2) { : come out of refractory mode
		on = 1
		m = 0
		net_send(firetime(), 1)
	}else{ : handle external event
		if (on) {
			m = m + w
			if (m >= 1) {
				net_move(t)
			}else{
				net_move(t+firetime())
			}
		}: else in refractory period
	}	
}

FUNCTION firetime()(ms) { : m < 1 and minf > 1
	if (minf > 1) {
		firetime = tau*log((minf-m)/(minf - 1))
	}else{
		firetime = 1e20
	}
:	printf("firetime=%g\n", firetime)
}

