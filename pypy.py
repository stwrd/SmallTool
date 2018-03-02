# -*- coding: utf-8 -*-
"""
Created on Fri Mar 02 05:22:02 2018

@author: Administrator
"""

import json
import os
import sys

from libs.save_mask_image import label_mask_writer
from libs.pascal_voc_io import PascalVocReader

def get_name_dic(file_path):
    with open(file_path) as infile:
        label_num_dic = json.load(infile)
        return label_num_dic

def generate_mask_png(label_num_dic,xml_file_name,output_path):
    if os.path.exists(output_path) is False:
        os.makedirs(output_path)
    tVocParseReader = PascalVocReader(xml_file_name)
    shapes = tVocParseReader.getShapes()
    image_size = tVocParseReader.get_img_size()
    
    num = 1
    no_ext_name = os.path.splitext(xml_file_name)
    
    for shape in shapes:
        final_path = no_ext_name + u'_' + unicode(num) + '.png'
        mask_writer = label_mask_writer(label_num_dic,final_path,image_size[0],image_size[1])
        mask_writer.save_mask_image(shapes) 
        num = num + 1


if __name__ == '__main__':
    folder_path = sys.argv[1]
    output_path = sys.argv[2]
    file_list = os.listdir(folder_path)
    print file_list
    label_num_dic = get_name_dic('label_num_dic.json')
    for file_name in file_list:
        xml_file_name = folder_path + '\\' + file_name
        generate_mask_png(label_num_dic,xml_file_name,output_path)
        
