#!/usr/bin/env python3
# encoding: utf-8

import csv
import matplotlib
import os
import pprint
import re
import sys

from matplotlib import pyplot
from matplotlib.backends.backend_pdf import PdfPages


def draw_graph(title, file, data):
    # Save Graphs on PDF
    f_ = os.path.join('graphs', file)
    p = PdfPages(f_)

    # Plot data
    for k in data.keys():
        pyplot.plot(data[k]['i'], data[k]['<'], label=('known (' + str(k) +
                                                       ')'))
        pyplot.plot(data[k]['i'], data[k]['||'], label=('unknown (' + str(k) +
                                                        ')'))

    pyplot.legend(loc=0)

    # Set title
    pyplot.title(title)
    pyplot.xlabel('Number of Philosopher')
    pyplot.ylabel('Number of Tries')

    pyplot.savefig(p, format='pdf')
    pyplot.close()
    p.close()
    # pyplot.show()

# Regular expression to get information from data filename
p = re.compile('(?P<events>\d+)\.(?P<threads>\d+)\.(?P<messages>\d+)\.out')

dir = 'data'
data = {}
ind_ = set()

for fn in os.listdir(dir):
    print(fn)

    # Get information from filename
    m = p.match(fn)
    if not m:
        print('warn: invalid file ' + fn + ', skipping...', file=sys.stderr)
        continue

    evt = int(m.group('events'))
    thr = int(m.group('threads'))
    msg = int(m.group('messages'))

    ind_.add(msg)

    # Read file
    f = open(os.path.join(dir, fn), 'r')
    values = csv.DictReader(f, fieldnames=['<', '||'], delimiter=' ')

    # Initialize variables
    cur = {}
    cur_evt = data.get(evt, {})
    count, less, unknown = 0, 0, 0

    # Compute average
    for d in values:
        less += int(d['<'])
        unknown += int(d['||'])
        count += 1
    less_avg = less / count
    unknown_avg = unknown / count

    # Save data
    cur['<'] = less_avg
    cur['||'] = unknown_avg
    cur_evt[(thr, msg)] = cur
    data[evt] = cur_evt

# Generate indices
ind = list(ind_)
ind.sort()
gdata = {}

# populate graph data
for e in data.keys():
    gdata[e] = {}
    for t, m in data[e].keys():
        gdata[e][t] = {'<': [], '||': [], 'i': []}

        # pprint.pprint(data[e])
        # print(t, m, data[e][(t, m)], data[e].get((t, m)))

        for m_ in ind:
            print(m, m_)
            x = data[e].get((t, m_))
            print(x)
            if x is None:
                continue
            gdata[e][t]['<'].append(x['<'])
            gdata[e][t]['||'].append(x['||'])
            gdata[e][t]['i'].append(m_)


pprint.pprint(gdata)

# Configure LaTeX fonts for graph
matplotlib.rc('text', **{'usetex': True, 'latex.unicode': True})
matplotlib.rc('font', **{'family': 'sans-serif', 'size': '14.0',
                         'sans-serif': ['Computer Modern Sans serif']})

for e in data.keys():
    draw_graph(str(e) + ' events', str(e) + '.pdf', gdata[e])
