import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from statsmodels.graphics.gofplots import qqplot

def load_data(file_path):
    return pd.read_csv(file_path)

def plot_total_attendance_by_district(data):
    total_attendance_district = data.groupby(by="District")[
        ['No. of facilities by performance - 1 to 100',
         'No. of facilities by performance - 101 to 500',
         'No. of facilities by performance - 501 to 1000',
         'No. of facilities by performance - >1000']
    ].sum()
    total_attendance_district.plot(kind='barh', figsize=(10, 6))
    plt.title('Total Patient Attendance for All Districts')
    plt.xlabel('Range Group')
    plt.ylabel('Total Attendance')
    plt.xticks(rotation=0)
    plt.tight_layout()
    plt.show()

def plot_facility_type_attendance(data):
    facility_type_attendance = data.groupby(['District', 'Facility Type'])[
        ['No. of facilities by performance - 1 to 100', 'No. of facilities by performance - 101 to 500',
         'No. of facilities by performance - 501 to 1000', 'No. of facilities by performance - >1000']
    ].sum().unstack()
    
    facility_type_attendance.plot(kind='barh', stacked=True, colormap='Set2')
    plt.xlabel("District")
    plt.ylabel("Total Patient Attendance")
    plt.title("Total Patient Attendance by Facility Type and Range Group")
    plt.xticks(rotation=0)
    plt.legend(title="Facility Type")
    plt.tight_layout()
    plt.show()

def plot_performance_average_by_district(data):
    selected_districts = ['Anantnag', 'Jammu', 'Poonch', 'Reasi', 'Udhampur']
    selected_data = data[data['District'].isin(selected_districts)]
    
    plt.figure(figsize=(10, 6))
    sns.barplot(data=selected_data, x='District', y='Performance - Overall Average **', hue='Facility Type', ci=None)
    plt.title('Performance - Overall Average by Facility Type')
    plt.xlabel('District')
    plt.ylabel('Performance - Overall Average')
    plt.xticks(rotation=45)
    plt.legend(title='Facility Type')
    plt.tight_layout()
    plt.show()

def plot_max_performance_for_districts(data):
    selected_districts = data['District'].sample(n=20)
    grouped_max_performance = data.groupby('District')['Performance - Maximum'].max().reset_index()
    
    plt.figure(figsize=(10, 6))
    sns.stripplot(data=grouped_max_performance[grouped_max_performance['District'].isin(selected_districts)], x='District', y='Performance - Maximum', jitter=True)
    plt.title('Performance - Maximum for Different Districts')
    plt.xlabel('District')
    plt.ylabel('Performance - Maximum')
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()

def plot_player_age_histogram(data):
    plt.figure(figsize=(10, 6))
    sns.histplot(data['Age'], kde=True)
    plt.title('Histogram and Kernel Density Plot of Player Age')
    plt.xlabel('Age')
    plt.ylabel('Frequency')
    plt.tight_layout()
    plt.show()

def plot_age_by_club(data):
    selected_clubs = ['FC Barcelona', 'Chelsea', 'Juventus', 'Real Madrid']
    filtered_data = data[data['Club'].isin(selected_clubs)]
    
    plt.figure(figsize=(10, 6))
    sns.kdeplot(data=filtered_data, x='Age', hue='Club', fill=True, common_norm=False, palette='tab10')
    plt.title('Kernel Density Plot of Player Age for Selected Clubs')
    plt.xlabel('Age')
    plt.ylabel('Density')
    plt.legend(title='Club')
    plt.tight_layout()
    plt.show()

def plot_player_value_distribution(data):
    def convert_value(value):
        value = value.replace(',', '')
        multiplier = 1
        if 'M' in value:
            multiplier = 1000
            value = value.replace('M', '')
        elif 'K' in value:
            value = value.replace('K', '')
        value = float(value.replace('$', '').replace('€', ''))
        return value * multiplier

    data['Value_in_K'] = data['Value'].apply(convert_value)
    
    sns.histplot(data=data, x='Value_in_K', hue='Preferred Foot', multiple='stack', palette='coolwarm', bins=100)
    plt.xlabel('Player Value (in Thousands)')
    plt.ylabel('Frequency')
    plt.title('Distribution of Player Values by Preferred Foot')
    plt.show()

def plot_international_reputation_qq_plot(data):
    standardized_international_reputation = (data["International Reputation"] - data["International Reputation"].mean()) / data["International Reputation"].std()
    qqplot(standardized_international_reputation, line='s')
    plt.show()

# Load Data
allopathic_attendance_data = load_data("JK-Allopathic-Outpatient_attendance-May-2019.csv")
fifa_data = load_data("Fifa_player_football_data.csv")

# Process and Plot Allopathic Data
plot_total_attendance_by_district(allopathic_attendance_data)
plot_facility_type_attendance(allopathic_attendance_data)
plot_performance_average_by_district(allopathic_attendance_data)
plot_max_performance_for_districts(allopathic_attendance_data)

# Process and Plot FIFA Data
plot_player_age_histogram(fifa_data)
plot_age_by_club(fifa_data)
plot_player_value_distribution(fifa_data)
plot_international_reputation_qq_plot(fifa_data)
