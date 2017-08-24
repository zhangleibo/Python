# -*- coding: utf-8 -*-
import requests
import re
import os
import urllib
import time
from bs4 import BeautifulSoup

'''
html_doc = """
<html><head><title>The Dormouse's story</title>
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>
</head>
<body>
<p class="title test"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
</body></html>
"""
# soup = BeautifulSoup(html_doc, 'html.parser')
soup = BeautifulSoup(html_doc, 'lxml')
"""
print soup.get_text()
print soup.title
print soup.title.string
print soup.title.text
print soup.title.parent.name
print soup.p.get_attribute_list('class')
for link in soup.find_all('a'):
    print link.get('href')
"""
rep = soup.select('a[href^="http://example.com/elsi"]')
for href in rep:
    print href
'''


def get_html_text(url):
    while 1:
        try:
            rep = requests.get(url).content.decode("utf-8", 'ignore')
        except requests.exceptions.ConnectionError:
            print "Connection Error"
            time.sleep(3)
            continue
        break

    return rep


def html_parse(html, selector):
    soup = BeautifulSoup(html, 'lxml')
    return soup.select(selector)


def get_sub_url(html):
    """"""
    selector = '#header > div > p > a'
    url_list = html_parse(html, selector)

    data = {}
    for href in url_list:
        data[href.get_text()] = href.get('href')
        # print href.get_text(), href.get('href')

    return data


def get_next_page(html, selector):
    url_list = html_parse(html, selector)
    if 1 == len(url_list):
        return url_list.pop(0).get('href')

    return ""


def get_biaoqingbao_url(html):
    data = {}
    while 1:
        selector = 'body > div.wrapper.whitebj.touxiangbox > div.w790 > ul > li > a.wz'
        url_list = html_parse(html, selector)

        for href in url_list:
            data[href.get_text()] = href.get('href')
            # print href.get_text(),  href.get('href')

        next_sel = 'body > div.wrapper.whitebj.touxiangbox > div.w790 > div > a.next'
        next_url = get_next_page(html, next_sel)
        if "" == next_url:
            break
        else:
            html = get_html_text(next_url)

    return data


def get_picture_url(html):
    """"""
    url_picture = []
    while 1:
        selector = 'body > div.wrapper.whitebj.pab25 > div.w780.biaoqingbox > ul > li > img'
        url_list = html_parse(html, selector)

        for tag in url_list:
            url_picture.append(tag.get('data-original'))
            # print tag.get('data-original')

        next_sel = 'body > div.wrapper.whitebj.pab25 > div.w780.biaoqingbox > div.bqfanye > div > a.next'
        next_url = get_next_page(html, next_sel)
        if "" == next_url:
            break
        else:
            html = get_html_text(next_url)

    return url_picture


def get_file_name(url):
    sub_str = re.split('/', url)

    for s in sub_str:
        if -1 != s.find(".jpg") \
                or -1 != s.find(".gif") \
                or -1 != s.find(".png") \
                or -1 != s.find(".bmp") \
                or -1 != s.find(".jpeg"):
            return s

    print "get file name file in [%s]" % url
    return ""


def down_load_picture(url, path):
    file_name = get_file_name(url)
    if "" == file_name:
        return

    create_dir_path(path)
    img_name = path + "/" + file_name
    try:
        urllib.urlretrieve(url, img_name)
    except IOError:
        print "Error: save picture file failed"


def create_dir_path(path):
    try:
        if not os.path.exists(path):
            os.makedirs(path)
    except WindowsError:
        print "WindowsError"


def is_path_exists(path):
    if not os.path.exists(path):
        return 0
    else:
        return 1


def process():
    root_path = u"微茶表情"
    host_url = "http://www.wxcha.com/biaoqing/"
    dict_url = get_sub_url(get_html_text(host_url))

    for key in dict_url:
        if u"微信表情专题" == key:
            continue

        class_path = root_path + "/" + key
        print "****** %s ****** " % key
        dict_bqb = get_biaoqingbao_url(get_html_text(dict_url[key]))

        cnt = 0
        for key1 in dict_bqb:
            path = class_path + "/" + key1
            try:
                print cnt, key1, dict_bqb[key1]
            except UnicodeEncodeError:
                print "UnicodeEncodeError"

            cnt += 1

            url_picture = get_picture_url(get_html_text(dict_bqb[key1]))

            if 1 == is_path_exists(path):
                continue

            for url in url_picture:
                down_load_picture(url, path)

# try:
#     process()
# except KeyboardInterrupt:
#     print "Stop"


def getPartialTable(pattern):
    if not pattern:
        return None
    lp = len(pattern)
    res = [-1, 0]
    if lp > 1:
        for curr in xrange(1, lp):
            ptr = curr
            while ptr > 0 and pattern[curr] != pattern[res[ptr]]:
                ptr = res[ptr]
            else:
                res.append(res[ptr] + 1)
    return res

def matchPatternKMP(string, pattern):
    if not string or not pattern:
        return None
    p_table = getPartialTable(pattern)
    start, matched = 0, 0
    ls, lp  = len(string), len(pattern)
    stop    = ls - lp + 1
    res     = list()
    while True:
        while matched == lp or string[start + matched] != pattern[matched]:
            if matched == lp:
                res.append(start)
            start   += matched - p_table[matched]
            matched  = max(0, p_table[matched])
            if not start < stop:
                return res
        else:
            matched += 1

if __name__ == '__main__':
    string  = 'abababaababacbababacb'
    pattern = 'aaa'
    print 'string:\t\t%s\npattern:\t%s' % (string, pattern)
    print matchPatternKMP(string, pattern)
