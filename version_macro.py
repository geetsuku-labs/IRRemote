import subprocess, datetime

revision = (
    subprocess.check_output(["git", "describe", "--match=''", "--always", "--abbrev=40", "--dirty"])
    .strip()
    .decode("utf-8")
)
print("-DGIT_REV='\"%s\"' -DBUILD_DATE='\"%s\"'" % (revision, datetime.datetime.now()))
