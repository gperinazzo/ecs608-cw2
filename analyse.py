#!/usr/bin/env python3
# encoding: utf-8

import csv
import matplotlib
import os
import pprint
import re
import sys

from matplotlib import pyplot as pp
from matplotlib.backends.backend_pdf import PdfPages


def draw_all(title, file, data):
    # Save Graphs on PDF
    f_ = os.path.join('graphs', file)
    p = PdfPages(f_)

    _keys = list(data.keys())
    _keys.sort()

    # Plot data
    for k in _keys:
        pp.plot(data[k]['i'], data[k]['<'], label=('known (' + str(k) + ')'))
        pp.plot(data[k]['i'], data[k]['||'], label=('unknown (' + str(k) +
                                                    ')'))

    pp.legend(loc=0)

    # Set title
    pp.title(title)
    pp.xlabel('Number of Communications')
    pp.ylabel('Number of Relations')

    pp.savefig(p, format='pdf')
    pp.close()
    p.close()
    # pyplot.show()


def draw_ratio(title, file, data):
    # Save Graphs on PDF
    f_ = os.path.join('graphs', file)
    p = PdfPages(f_)

    _keys = list(data.keys())
    _keys.sort()

    # Plot data
    for k in _keys:
        pp.plot(data[k]['i'], data[k]['r'], label=(str(k) + ' threads'))

    pp.legend(loc=0)

    # Set title
    pp.title(title)
    pp.xlabel('Number of Communications')
    pp.ylabel('Ratio of Known Relations')

    pp.savefig(p, format='pdf')
    pp.close()
    p.close()
    # pyplot.show()


def draw(title, file, data):
    # Save Graphs on PDF
    f_ = os.path.join('graphs', file)
    p = PdfPages(f_)

    # Plot data
    pp.plot(data['i'], data['<'], label='known')
    pp.plot(data['i'], data['||'], label='unknown')

    pp.legend(loc=0)

    # Set title
    pp.title(title)
    pp.xlabel('Number of Communications')
    pp.ylabel('Number of Relations')

    pp.savefig(p, format='pdf')
    pp.close()
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
        gdata[e][t] = {'<': [], '||': [], 'i': [], 'r': []}

        # pprint.pprint(data[e])
        # print(t, m, data[e][(t, m)], data[e].get((t, m)))

        for m_ in ind:
            x = data[e].get((t, m_))
            if x is None:
                continue
            gdata[e][t]['<'].append(x['<'])
            gdata[e][t]['||'].append(x['||'])
            gdata[e][t]['i'].append(m_)
            gdata[e][t]['r'].append(x['<'] / (x['<'] + x['||']))


pprint.pprint(gdata)

# Configure LaTeX fonts for graph
matplotlib.rc('text', **{'usetex': True, 'latex.unicode': True})
matplotlib.rc('font', **{'family': 'sans-serif', 'size': '14.0',
                         'sans-serif': ['Computer Modern Sans serif']})

for e in data.keys():
    draw_all(str(e) + ' events', str(e) + '.pdf', gdata[e])
    draw_ratio(str(e) + ' events', str(e) + '.ratio.pdf', gdata[e])
    for t in gdata[e].keys():
        title = str(e) + ' events with ' + str(t) + ' threads'
        filename = str(e) + '.' + str(t) + '.pdf'
        draw(title, filename, gdata[e][t])
