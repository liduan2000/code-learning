from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

zipurl_prefix = "https://s3.amazonaws.com/tripdata/"
zipurl_suffix = ".zip"
for i in range(1, 13):
    file_name = "2022"
    if 6 <= i <= 7:
        file_name += "0" + str(i) + "-citbike-tripdata.csv"
    elif i < 10:
        file_name += "0" + str(i) + "-citibike-tripdata.csv"
    elif i >= 10:
        file_name += str(i) + "-citibike-tripdata.csv"
    zipurl = zipurl_prefix + file_name + zipurl_suffix
    with urlopen(zipurl) as zipresp:
        with ZipFile(BytesIO(zipresp.read())) as zfile:
            zfile.extractall("/tmp/")
            dbutils.fs.mv("file:/tmp/" + file_name, "dbfs:/tmp/" + file_name)
