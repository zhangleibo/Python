# -*- coding: utf-8 -*-

import os
import re
import sys
import urllib
import urllib2
import cookielib


def func():
    """ POST """
    url = "https://www.douban.com/"
    data = {'source': 'index_nav',
            'admin_account': '18629640325',
            'password': '13772492775',
            'captcha-solution': 'station',
            'captcha-id': 'pFfIzKTReYmlg5xhL1JVmDx2:en'}

    cookie_support = urllib2.HTTPCookieProcessor(cookielib.CookieJar())
    opener = urllib2.build_opener(cookie_support, urllib2.HTTPHandler)

    resp = opener.open(url)
    print resp.read()

    data = urllib.urlencode(data)
    req = urllib2.Request(url=url, data=data)
    response = urllib2.urlopen(req)

    print response.read()


if __name__ == '__main__':
    print "start..."
    func()
    print "end..."
