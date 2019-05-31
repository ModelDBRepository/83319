COMMENT
Total current is b + weighted events that decay with time constants taue and taui.
Total current is integrated, and when this passes 1, then fire.
That is

taue * de/dt + e = 0
taui * di/dt + i = 0
  where an event at time t1 causes e or i to be incremented by amount w, 
  depending on sign of w, i.e.
  if (w > 0) then e += w
  else i += w
  and
taum*dm/dt + m = ae*e + ai*i + b
  where fire if m crosses 1 in a positive direction
  and after firing m is reset to 0

Requires that taue <= taui, but taum can be larger or smaller than either 
or both of the synaptic time constants.
ENDCOMMENT

NEURON {
	ARTIFICIAL_CELL IF3
	RANGE taue, taui, taum, e, i, m, b
	RANGE nself, nexcite, ninhibit
	GLOBAL eps, refrac
}

PARAMETER {
	: specify limits on these
	taue = 3 (ms) < 1e-9, 1e9 >
	taui = 10 (ms) < 1e-9, 1e9 >
	taum = 30 (ms) < 1e-9, 1e9 >
	b = 0
	eps = 1e-6
	refrac = 5 (ms)
}

ASSIGNED {
	e i m
	enew inew mnew
	t0 (ms)
	nself nexcite ninhibit

	ae ai
	be bi
	on
}

: argument is the interval since last update
PROCEDURE newstates(d(ms)) {
	LOCAL ee, ei, em

	ee = exp(-d/taue)
	ei = exp(-d/taui)
	em = exp(-d/taum)

	enew = e*ee
	inew = i*ei
	mnew = b + (m - b)*em
		+ ae*e*(taue/(taum - taue))*(em - ee)
		+ ai*i*(taui/(taum - taui))*(em - ei)
}

FUNCTION M() {
	if (on) {
		newstates(t - t0)
		M = mnew
	}else{
		M = 0
	}
}

FUNCTION E() {
	newstates(t - t0)
	E = enew
}

FUNCTION I() {
	newstates(t - t0)
	I = inew
}

PROCEDURE update() {
	e = enew
	i = inew
	m = mnew
	t0 = t
}

PROCEDURE factors() {
	LOCAL tp
	: force all exponential solution ( no x*exp(-x) )
	: and force assertion for correctness of algorithm
	: i.e. taue is smallest and only one that is excitatory
	if (taue >= taui) { taui = taue + 0.01 }
	if (taum == taue) { taum = taue + 0.01 }
	if (taum == taui) { taum = taui + 0.01 }

	: normalize so the peak magnitude of m is 1 when single e of 1
	tp = log(taue/taum)/((1/taum) - (1/taue))
	be = 1/(exp(-tp/taum) - exp(-tp/taue))
	ae = be*((taum/taue) - 1)

	: normalize so the peak magnitude of m is -1 when single i of -1
	tp = log(taui/taum)/((1/taum) - (1/taui))
	bi = 1/(exp(-tp/taum) - exp(-tp/taui))
	ai = bi*((taum/taui) - 1)
}

FUNCTION tf(bb) (ms) {
	if (bb > 1) { tf = taum*log((bb-m)/(bb-1)) }
	else { tf = 1e9 }
}

FUNCTION firetimebound() (ms) {
	LOCAL h, temp
	h = ae*e + ai*i
	if (b>1) {
		if (h>0) {
			firetimebound = tf(h+b)
		} else {
			: take net inhibitory current into account, at least partially
			temp = tf(b) : too early
			temp = tf(b + h*exp(-temp/taui)) : too late
			: later than too early, but not too late
			firetimebound = tf(b + h*exp(-temp/taui))
		}
	} else {
		: this compound conditional can be simplified
:		if ((h+b-m > 0) && (h+b > 1)) {
		: h+b-m > 0 implies h+b > m, but h+b > 1 guarantees h+b > m since 1 >= m
		if (h+b > 1) {
			firetimebound = tf(h+b)
		} else {
			firetimebound = 1e9
		}
	}
:	printf("firetimebound=%g\n", firetimebound)
}

LOCAL total, nexttotal

INITIAL {
	factors()
	e = 0
	i = 0
	m = 0
	t0 = t
	on = 1
	net_send(firetimebound(), 1)

	nself=0
	nexcite=0
	ninhibit=0
	total = 0
	nexttotal = 1000
}

NET_RECEIVE (w) {
	newstates(t-t0)
	update()	
:printf("event %g t=%g e=%g i=%g m=%g\n", flag, t, e, i, m)
	if (flag == 1) { :determine whether to fire or try again
		nself = nself + 1
		if (m > 1-eps) { : time to fire
			if (m > 1+eps) { : oops!
				printf("m>1 error in IF3 mechanism--m = %g\n", m)
				: how to quit?
			}
			:printf("fire %g\n", t)
			net_event(t)
			on = 0
			m = 0
			net_send(refrac, 2) : start refractory state
			total = total + 1
			if (total == nexttotal) {
:				printf("IF3 total spikes %g at %g\n", total, t)
				nexttotal = nexttotal + 1000
			}
		}else{ : try again
			net_send(firetimebound(), 1)
		}
	}else if (flag == 2) { : end of refractory period
		on = 1
		m = 0
		net_send(firetimebound(), 1)
	} else {
		if (w > 0) {
			nexcite = nexcite + 1
			e = e + w
		} else {
			ninhibit = ninhibit + 1
			i = i + w
		}
:printf("w=%g e=%g i=%g\n", w, e, i)
		if (on) {
			net_move(firetimebound() + t)
		}
	}
}
