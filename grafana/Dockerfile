FROM grafana/grafana-oss:latest
ENV GF_SECURITY_DISABLE_INITIAL_ADMIN_CREATION="true"
ENV GF_AUTH_ANONYMOUS_ENABLED="true"
ENV GF_AUTH_ANONYMOUS_ORG_ROLE="Admin"
ENV GF_AUTH_DISABLE_SIGNOUT_MENU="true"
ENV GF_AUTH_DISABLE_LOGIN_FORM="true"
#ENV GF_INSTALL_PLUGINS="https://algenty.github.io/flowcharting-repository/archives/agenty-flowcharting-panel-0.5.0.zip;grafana-flowcharting"
ENV GF_INSTALL_PLUGINS="agenty-flowcharting-panel"
ENV GF_DASHBOARDS_DEFAULT_HOME_DASHBOARD_PATH="/etc/grafana/provisioning/dashboards/home.json"
COPY provisioning/datasources.yaml /etc/grafana/provisioning/datasources/datasources.yml
COPY provisioning/dashboards/ /etc/grafana/provisioning/dashboards/
