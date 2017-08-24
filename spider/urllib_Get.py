# -*- coding: utf-8 -*-
import os
import re
import sys
import urllib
import urllib2


def func():
    url = "http://www.baidu.com/s"
    data = {'wd': '宁哥的小站'}

    data = urllib.urlencode(data)
    full_url = url + '?' + data
    print full_url

    response = urllib.urlopen(full_url)

    print response.read()

if __name__ == '__main__':
    print "start..."
    func()
    print "end..."
