# -*- coding: utf-8 -*-
import os
import sys
import urllib2
import requests
import re
import urllib
import time
from lxml import etree


def StringListSave(save_path, filename, slist):
    if not os.path.exists(save_path):
        os.makedirs(save_path)
    path = save_path + "/" + filename + ".txt"
    with open(path, "w+") as fp:
        for s in slist:
            fp.write("%s\t\t%s\n" % (s[0].encode("utf8"), s[1].encode("utf8")))


def PictureListSave(save_path, filename, slist):
    if not os.path.exists(save_path):
        os.makedirs(save_path)
    path = save_path + "/" + filename + ".txt"
    with open(path, "w+") as fp:
        for s in slist:
            fp.write("%s\t\t%s\n" % (s[0].encode("utf8"), s[1].encode("utf8")))


def save_picture(tmp_path, new_page_info, index):
    img_cnt = index
    for url2, item2 in new_page_info:
        img_cnt += 1
        end_file = "_picture"
        picture_name = re.findall(r'http://.*?/(.*?).gif', url2, re.S)
        if 0 == len(picture_name):
            picture_name = re.findall(r'http://.*?/(.*?).jpg', url2, re.S)
            end_file += ".jpg"
        else:
            end_file += ".gif"

        img_name = tmp_path + '/' + str(img_cnt) + end_file

        try:
            urllib.urlretrieve(url2, img_name)
        except IOError:
            print "Error: write file failed"

    # r = requests.get(url2)
    # with open(img_name, 'wb') as f:
    #    f.write(r.content)

    return img_cnt


def find_next_page(new_page):
    next_all_url = re.findall(r'<div class="pages">(.*?)</div>', new_page, re.S)
    if 0 == len(next_all_url):
        return ""

    next_split_url = re.findall(r'<a (.*?)</a>', next_all_url.pop(0), re.S)
    if 0 == len(next_split_url):
        return ""

    for url in next_split_url:
        next_url = re.findall(r'href="(.*?)" class="next" title=".*?">.*?', url, re.S)
        if 0 == len(next_url):
            continue
        else:
            return next_url.pop(0)

    return ""


def process_picture_info(path, new_page_info, index):
    j = index
    for url, item in new_page_info:
        j += 1
        tmp_path = path + '/' + str(j) + '_' + item

        if not os.path.exists(tmp_path):
            os.makedirs(tmp_path)
        else:
            return j

        print "downloading ", j, url

        next_url = url
        img_cnt = 0

        while 1:
            new_page = requests.get(next_url).content.decode("utf-8")
            new_page_ret = New_Page_Info1(new_page)
            img_cnt = save_picture(tmp_path, new_page_ret, img_cnt)
            next_url = find_next_page(new_page)
            if "" == next_url:
                break

    return j


def Page_Info(myPage):
    '''Regex'''
    # mypage_Info = re.findall(r'<div class="titleBar" id=".*?"><h2>(.*?)</h2><div class="more"><a href="(.*?)">.*?</a></div></div>', myPage, re.S)
    mypage_Info = re.findall(r'<p class="subnav">(.*?)</p>', myPage, re.S)
    class_Info = re.findall(r'<a href="(.*?)">(.*?)</a>', mypage_Info.pop(0), re.S)
    return class_Info


def New_Page_Info(new_page):
    '''Regex(slowly) or Xpath(fast)'''
    # new_page_Info = re.findall(r'<td class=".*?">.*?<a href="(.*?)\.html".*?>(.*?)</a></td>', new_page, re.S)
    # # new_page_Info = re.findall(r'<td class=".*?">.*?<a href="(.*?)">(.*?)</a></td>', new_page, re.S) # bugs
    # results = []
    # for url, item in new_page_Info:
    #     results.append((item, url+".html"))
    # return results

    new_page_Info = re.findall(
        r'<p class="ms3 cl"><a href="javascript:;">.*?</a><a href="javascript:;" class="hate">.*?</a></p>.*?<a href="(.*?)" class="wz" target="_blank">(.*?)</a>',
        new_page, re.S)
    return new_page_Info


    # new_items = dom.xpath('//tr/td/a/text()')
    # new_urls = dom.xpath('//tr/td/a/@href')
    # assert(len(new_items) == len(new_urls))
    # dom = etree.HTML(new_page)
    # return zip(new_items, new_urls)


def New_Page_Info1(new_page):
    '''Regex(slowly) or Xpath(fast)'''
    # new_page_Info = re.findall(r'<td class=".*?">.*?<a href="(.*?)\.html".*?>(.*?)</a></td>', new_page, re.S)
    # # new_page_Info = re.findall(r'<td class=".*?">.*?<a href="(.*?)">(.*?)</a></td>', new_page, re.S) # bugs
    # results = []
    # for url, item in new_page_Info:
    #     results.append((item, url+".html"))
    # return results

    new_page_Info = re.findall(r'<li><img class="lazy" src=".*?" data-original="(.*?)" alt="(.*?)" /></li>', new_page,
                               re.S)
    return new_page_Info


def find_next_url(new_page):
    next_url = re.findall(r'<a href="(.*?)" class="next" title=".*?">.*?</a>', new_page, re.S)
    return next_url.pop(0)


def spider(url):
    i = 0
    print u"微茶表情全集下载", url
    response = requests.get(url)
    content = requests.get(url).content

    host_page = requests.get(url).content.decode("utf-8")
    # myPage = urllib2.urlopen(url).read().decode("gbk")
    host_page_ret = Page_Info(host_page)
    save_path = u"微茶表情抓取"
    if not os.path.exists(save_path):
        os.makedirs(save_path)

    for url, item in host_page_ret:
        tmp_path = save_path + '/' + str(i) + '_' + item
        i += 1

        if not os.path.exists(tmp_path):
            os.makedirs(tmp_path)
        else:
            continue

        print "downloading ", item, url

        next_url = url
        index = 0
        while 1:
            try:
                new_page = requests.get(next_url).content.decode("utf-8")
                new_page_ret = New_Page_Info(new_page)
                index = process_picture_info(tmp_path, new_page_ret, index)
                next_url = find_next_page(new_page)
                if "" == next_url:
                    break
            except:
                print "Error except..."
            else:
                continue


if __name__ == '__main__':
    print "start"
    start_url = "http://www.wxcha.com/biaoqing/"

    while 1:
        print time.ctime()
        try:
            spider(start_url)
        except requests.exceptions.ConnectionError:
            print "Connection Error"

        time.sleep(3)

    print "end"
